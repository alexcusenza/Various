/* ============================================================================
 * Name        : Hello.cpp
 * Author      : Wayne Ross
 * Version     :
 * Copyright   : 2013 Wayne Ross
 * Description : Hello World in C++, Ansi-style
 *
 * ==============================================================================
 */

#include <stdio.h> // open()
#include <stdlib.h> // exit()
#include <fcntl.h> // O_NOCTTY, O_RDWR, open(),
#include <termios.h>
#include <string.h> // memset()
#include <errno.h>
//#include <sys/stat.h>
//#include <sys/types.h>
#include <fstream>
#include <getopt.h>
#include <unistd.h>
#include <time.h>

#include "CFwStdioProtocol.h"


// CAN relevant portions
#include <sys/ioctl.h>
#include "../advcan_source_v2.17/driver/can4linux.h"

// PORT DEFINITIONS
#define BAUDRATE B38400
#define MODEMDEVICE "/dev/ttyS0"
#define STDDEV "can0" // Labeled as "CH#2" on the circuit board

int can_fd;
canmsg_t message;
fd_set rset;


/* *************************
 * FUNCTIONS
 * *************************/

void TimeSpecSubtract(timespec *result, timespec *x, timespec *y)
{
	int NanoSecInSec = 1000000000; // 1,000,000,000

	/* Perform the carry for the later subtraction by updating y. */
	if (x->tv_nsec < y->tv_nsec)
	{
		int nsec = (y->tv_nsec - x->tv_nsec) / NanoSecInSec + 1;
		y->tv_nsec -= NanoSecInSec * nsec;
		y->tv_sec += nsec;
	}

	if (x->tv_nsec - y->tv_nsec > NanoSecInSec)
	{
		int nsec = (x->tv_nsec - y->tv_nsec) / NanoSecInSec;
		y->tv_nsec += NanoSecInSec * nsec;
		y->tv_sec -= nsec;
	}

	result->tv_sec = x->tv_sec - y->tv_sec;
	result->tv_nsec = x->tv_nsec - y->tv_nsec;
}


/* *************************
 * MAIN
 * *************************/

int main(int argc, char **argv)
{
	int fd, res;
	struct termios oldtio,newtio;
	char buf[255];

	int MyPolicy = SCHED_FIFO;
	sched_param MySchedParam;
	sched_getparam(0, &MySchedParam);
	MySchedParam.sched_priority = (sched_get_priority_max(MyPolicy) + sched_get_priority_min(MyPolicy)) / 2;

	int vminvalue = 1;
	bool spin = false;
	bool CAN = false;
	bool txmode = false;
	bool infmode = false;
	bool printscreen = false;
	bool help = false;
	char c;

	/*
	 * COMMAND LINE INTERFACE OPTIONS
	 */
	while ((c = getopt (argc, argv, "hitscdv:p:")) != -1)
	{
		switch (c)
		{
		case 'h':
			help = true;
			break;

		case 's':
			spin = true;
			break;

		case 'v':
			vminvalue = atoi(optarg);
			break;

		case 'c':
			CAN = true;
			break;

		case 'p':
			if (atoi(optarg) == 0)
				MyPolicy = SCHED_OTHER;
			else
				MySchedParam.sched_priority = atoi(optarg);
			break;

		case 'd':
			printscreen = true;
			break;

		case 't':
			txmode = true;
			break;

		case 'i':
			infmode = true;
			//MySchedParam.sched_priority = 1;
			MyPolicy = SCHED_OTHER;
			break;

		case '?':
			if (isprint (optopt))
				fprintf (stderr, "Unknown option `-%c'.\n", optopt);
			else
				fprintf (stderr, "Unknown option character `\\x%x'.\n", optopt);
			return 1;

		default:
			abort ();
		}
	}

	/*
	 * SCHEDULE PRIORITY INITIALIZATION
	 */

	if (MyPolicy == SCHED_FIFO)
	{
		printf("Requesting Policy %d (-l VALUE) at priority (-p VALUE) %d\n", MyPolicy, MySchedParam.sched_priority);

		if (sched_setscheduler(0, MyPolicy, &MySchedParam) < 0)
		{
			if (errno == EACCES)
				fprintf(stderr,"\nrun as sudo!\n\n");
			else
				fprintf(stderr,"Error %d '%s' on sched_setscheduler()\n", errno, strerror(errno));
			exit(-1);
		}
	}
	else
		printf("Utilizing scheduling policy SCHED_OTHER (-p VALUE = 0), not RT\n");

	printf("Hello World! (and serial port %s)\n",MODEMDEVICE);

	/*
	 * Open modem device for reading and writing and not as controlling tty
	 * because we don't want to get killed if linenoise sends CTRL-C.
	 */
	fd = open(MODEMDEVICE, O_RDWR | O_NOCTTY );
	if (fd <0) 
	{
		if (errno == EACCES)
			fprintf(stderr,"\nrun as sudo!\n\n");
		else
			fprintf(stderr,"Error %d '%s' opening SERIAL device %s\n", errno, strerror(errno), MODEMDEVICE);

		exit(-1);
	}

	/*
	 * TERMIOS OPTIONS
	 */
	tcgetattr(fd,&oldtio); 					// save current serial port settings
	memset(&newtio, 0, sizeof(newtio)); 	//clear struct for new port settings

	// TERMIOS CONTROL OPTIONS
	cfsetispeed(&newtio, BAUDRATE);
	cfsetospeed(&newtio, BAUDRATE);

	newtio.c_cflag |= (CLOCAL | CREAD | CS8);

	// TERMIOS LINE OPTIONS
	if (spin)
		newtio.c_lflag |= ICANON;
	else
		newtio.c_lflag &= ~ICANON;

	// TERMIOS INPUT OPTIONS
	newtio.c_iflag |= IGNPAR;  //| ICRNL;

	// TERMIOS OUTPUT OPTIONS
	newtio.c_oflag = 0;

	// TERMIOS CONTROL CHARACTERS
	if (!spin)
	{
		newtio.c_cc[VTIME]    = 0;
		newtio.c_cc[VMIN]     = vminvalue;     /* blocking read until 1 character arrives */
	}

	// TERMIOS SET ATTRIBUTES
	tcflush(fd, TCIFLUSH);
	tcsetattr(fd,TCSANOW,&newtio);
	//tcsetattr(fd, TCSAFLUSH, &newtio);

	/*
	 * TERMIOS END
	 */

	int status;
	ioctl(fd, TIOCMGET, &status); // current modem state bits

	const char HW[] = "Hello Serial Port!";
	const char CR[] = "\015"; // \m or decimal 13
	const char LF[] = "\012"; // \j or decimal 10

	const char TX[] = "abcdefghijklmnopqrs";	// add two for [cr][lf]
	//const char TX[] = "xxxxx1.2345678xxxxZ";

	res = write(fd,HW,strlen(HW));
	res = write(fd,CR,strlen(CR));
	res = write(fd,LF,strlen(LF));

	setbuf(stdout, NULL); // disable output buffering

	Send_par_t SendTeil;
	if (CAN)
	{
		char device[40] = STDDEV;
		/* our default 8 byte message */
		message.id      = 0x80; // SYNC
		message.cob     = 0;
		message.flags   = 0;
		message.length  = 0;
		message.data[0] = 0x55;
		message.data[1] = 0; // used to count messages: will be incremented after each transmit
		message.data[2] = 3;
		message.data[3] = 4;
		message.data[4] = 5;
		message.data[5] = 6;
		message.data[6] = 7;
		message.data[7] = 0xaa;
		sprintf(device, "/dev/%s", STDDEV);

		if(( can_fd = open(device, O_RDWR )) < 0 )
		{
			if (errno == EACCES)
				fprintf(stderr,"\nrun as sudo!\n\n");
			else
				fprintf(stderr,"Error %d '%s' opening CAN device %s\n", errno, strerror(errno), device);

			exit(1);
		}
		SendTeil.Tx = &message;
	}

	// Let's use Framework's methods to check for local keyboard input
	// in order to terminate task gracefully
	CFwStdioProtocol* m_pInput;  // input object reference
	m_pInput = new CFwStdioProtocol();

#define NUMBINS 24

	int32u m_adwIrqLoopBin[NUMBINS];

	for (int q=0; q<NUMBINS; q++)
		m_adwIrqLoopBin[q] = 0;

	int q = 0;
	int cnt;

	/*
	 * PRINT OUT MODE OF OPERATION
	 */
	if (help)
		printf(" HELP: Displaying Program options\n");

	else if (txmode)
	{
		printf("In transmit mode, sending message...\n");
		printf(" %s\n", TX);
	}
	else if (infmode)
		printf("In infinite loop mode....\n");
	else
	{
		if (spin)
			printf("CANONICAL MODE: blocking read until [CR]...\n");
		else
		{
			printf("NON-CANONICAL MODE: blocking read until VMIN = %d (-v VALUE)\n", vminvalue);
		}
	}

	struct timespec timecurr;
	struct timespec timeprev;
	struct timespec timeresult;
	struct timespec timeread;
	//struct timespec time_canstart;
	struct timespec time_canend;
	struct timespec time_msgpacket_curr, time_msgpacket_prev;

	//float timecurr_entry=0, timeprev_entry=0, timemax_entry=0, timemin_entry=0, timeavg_entry=0;
	float valtimeres1 = 0;
	float maxtimeresult1 = 0;
	float mintimeresult1 = 0;
	float avgtimeres1 = 0;
	float valtimeres1_prev = 0;

	float timecurr_readblock=0, timeprev_readblock=0, timemax_readblock=0, timemin_readblock=0, timeavg_readblock=0;
	float timecurr_canmsg=0, timeprev_canmsg=0, timemax_canmsg=0, timemin_canmsg=0, timeavg_canmsg=0;
	float timecurr_msgpacket=0, timeprev_msgpacket=0, timemax_msgpacket=0, timemin_msgpacket=0, timeavg_msgpacket=0;

	int alpha = 0.9;

	bool running = true;
	bool infloop = true;
	bool canmsgtoggle = false;
	//int canmsgb1inc = 0;
	//int canmsgb2inc = 0;
	long long datacnt = 0;

	clock_gettime(CLOCK_MONOTONIC, &timeprev);
	time_msgpacket_curr = timeprev;
	//clock_gettime(CLOCK_MONOTONIC, &time_msgpacket_curr);
	tcflush(fd, TCIFLUSH);

	while (running)
	{
		/*
		 * HELP DISPLAY
		 */
		if (help == true)
		{
			printf("\n");
			printf(" -s: CANONICAL (read on [CR]) otherwise Non-Canonical (set number of bytes to read using -v option, default = 1\n");
			printf(" -t: TRASMIT mode - device will send out data packets, otherwise will receive\n");
			printf(" -i: INFINITE mode - programs enters a 100 Percent CPU utilization task.  Run in a separate terminal\n");
			printf(" -v: VMIN - sets number of bytes to read from serial port\n");
			printf(" -p: PRIORITY - value of p1 to p99 will set RT Preempt priority, p0 disables and SCHED_OTHER\n");
			printf(" -c: CAN mode - will transmit data over CAN bus upon [CR]\n");

			running = false;
		}


		/*
		 * INFINITE LOOP MODE
		 * Used to generate a 100% CPU utilization task
		 */
		if (infmode == true)
		{
			while (infloop)
			{
				cnt = cnt + 1;

				if (m_pInput->GetChar() != -1)
				{
					infloop = false;
					running = false;
					break;
				}
			}
		}
		/*
		 * TRANSMIT MODE:
		 * Used for transmitting serial data to mimic gyro or such
		 */
		else if (txmode == true)
		{
			write(fd,TX,strlen(TX));
			write(fd,CR,strlen(CR));
			write(fd,LF,strlen(LF));
		}
		/*
		 * NORMAL READ PACKET INFORMATION MODE
		 */
		else
		{
			// Measure time duration for read block
			clock_gettime(CLOCK_MONOTONIC, &timecurr);
			res = read(fd, buf, 255);
			clock_gettime(CLOCK_MONOTONIC, &timeread);

			if (res)
			{
				q += res; 			// tally count of characters seen
				++m_adwIrqLoopBin[min(res, NUMBINS-1)];

				// Data in buffer, cycle through all characters, and look for [CR] or [LF]
				for (int g=0; g<res; g++)
				{
					if (buf[g] == LF[0])	// specifically got a LF
					{
						// Measure time duration between successive [CR] reads
						time_msgpacket_prev = time_msgpacket_curr;
						clock_gettime(CLOCK_MONOTONIC, &time_msgpacket_curr);
						TimeSpecSubtract (&timeresult, &time_msgpacket_curr, &time_msgpacket_prev);
						timecurr_msgpacket = timeresult.tv_sec + timeresult.tv_nsec/1000000000.0;

						// If CAN device connected, send out SYNC message
						if (CAN)
						{
							if (canmsgtoggle)
							{
								canmsgtoggle = false;
								message.id      = 0x80; // SYNC
								message.cob     = 0;
								message.flags   = 0;
								message.length  = 0;
								message.data[0] = 0x55;
								message.data[1] = 0;
								message.data[2] = 3;
								message.data[3] = 4;
								message.data[4] = 5;
								message.data[5] = 6;
								message.data[6] = 7;
								message.data[7] = 0xaa;
							}
							else
							{
								canmsgtoggle = true;
								message.id      = 0x11;
								message.cob     = 11;
								message.flags   = 0;
								message.length  = 8;

								datacnt++;
								memcpy(&message.data[0], &datacnt, 8);
							}

							cnt = ioctl(can_fd, CAN_IOCTL_SEND, &SendTeil);
							// Measure time duration between read block exit and just after CAN synce message is sent
							clock_gettime(CLOCK_MONOTONIC, &time_canend);
							TimeSpecSubtract (&timeresult, &time_canend, &timeread);
							timecurr_canmsg = timeresult.tv_sec + timeresult.tv_nsec/1000000000.0;

						}

						//q = 0;
						// printf(" :%d", q); // one time per line
						// write(fd, CR, strlen(CR));
					} // if()
				} // for()

				buf[res]=0;             	// set end of string, so we can printf
				//write(fd,buf,res); 		// echo content back out serial port

				// calculate time duration of program cycle time
				TimeSpecSubtract (&timeresult, &timecurr, &timeprev);
				valtimeres1 = timeresult.tv_sec + timeresult.tv_nsec/1000000000.0;

				// calculate time duration of read block
				TimeSpecSubtract (&timeresult, &timeread, &timecurr);
				timecurr_readblock = timeresult.tv_sec + timeresult.tv_nsec/1000000000.0;

				/*
				 * EXIT-ENTRY PROGRAM TIMING
				 */
				// Maximum calculation
				if (valtimeres1 > maxtimeresult1)
					maxtimeresult1 = valtimeres1;

				// Minimum calculation
				if (mintimeresult1 == 0 || valtimeres1 < mintimeresult1)
					mintimeresult1 = valtimeres1;

				// Moving-average calculation: lowpass digital filter algorithm
				avgtimeres1 = alpha * valtimeres1_prev + (1-alpha) * valtimeres1;
				valtimeres1_prev = avgtimeres1;

				/*
				 * READ BYTE TIMING
				 */
				if (timecurr_readblock > timemax_readblock)
				{
					timemax_readblock = timecurr_readblock;
					//printf("Encountered MAX %f (avg:%f)\n", timemax_readblock, timeavg_readblock);
				}
				if (timemin_readblock == 0 || timecurr_readblock < timemin_readblock)
				{
					timemin_readblock = timecurr_readblock;
					//printf("Encountered MIN %f \n", timemin_readblock);
				}

				timeavg_readblock = alpha * timeprev_readblock + (1-alpha) * timecurr_readblock;
				timeprev_readblock = timeavg_readblock;

				/*
				 * CAN TIMING
				 */
				if (timecurr_canmsg > timemax_canmsg)
					timemax_canmsg = timecurr_canmsg;

				if (timemin_canmsg == 0 || timecurr_canmsg < timemin_canmsg)
					timemin_canmsg = timecurr_canmsg;

				timeavg_canmsg = alpha * timeprev_canmsg + (1-alpha) * timecurr_canmsg;
				timeprev_canmsg = timeavg_canmsg;

				/*
				 * READ PACKET TIMING
				 */
				if (timecurr_msgpacket > timemax_msgpacket)
					timemax_msgpacket = timecurr_msgpacket;

				if (timemin_msgpacket == 0 || timecurr_msgpacket < timemin_msgpacket)
					timemin_msgpacket = timecurr_msgpacket;

				timeavg_msgpacket = alpha * timeprev_msgpacket + (1-alpha) * timecurr_msgpacket;
				timeprev_msgpacket = timeavg_msgpacket;

				// On-screen print mode
				if (printscreen)
				{
					if (spin)
						printf(" %1.6f, %1.6f, %d, %s", (valtimeres1)*1000., (timecurr_readblock)*1000., res, buf);
					else
					{
						if (res > 1)
							printf(" %1.6f, %1.6f, %d\n", (valtimeres1)*1000., (timecurr_readblock)*1000., res);
					}
				}
			} // if (res)
		}

		clock_gettime(CLOCK_MONOTONIC, &timeprev);

		if (m_pInput->GetChar() != -1)
			break; // some key was pushed locally

	} // while()


	/*
	 * PRINT OUT RESULTS
	 */
	if (txmode == false && infmode == false && help == false)
	{
		printf("\nNumber of chars received at a time:\n");

		for (int q=0; q<NUMBINS; q++)
		{
			if (m_adwIrqLoopBin[q] != 0)
				printf("(%d):%d, ",q,m_adwIrqLoopBin[q]);
		}

		printf("\n\n");
		printf("cycle wait time  AVG:%1.3f (us), MIN:%1.3f (us), MAX:%2.3f (us)\n", avgtimeres1*1000000, mintimeresult1*1000000, maxtimeresult1*1000000);
		printf("read block time  AVG:%1.3f (ms), MIN:%1.3f (ms), MAX:%2.3f (ms)\n", timeavg_readblock*1000, timemin_readblock*1000, timemax_readblock*1000);
		printf("[CR] packet time AVG:%1.3f (ms), MIN:%1.3f (ms), MAX:%2.3f (ms)\n", timeavg_msgpacket*1000, timemin_msgpacket*1000, timemax_msgpacket*1000);
		printf("can output time  AVG:%1.3f (us), MIN:%1.3f (us), MAX:%2.3f (us)\n", timeavg_canmsg*1000000, timemin_canmsg*1000000, timemax_canmsg*1000000);
	}

	/*
	 * RESTORE AND CLOSE PORTS
	 */

	tcsetattr(fd,TCSANOW,&oldtio);

	delete m_pInput;

	if (CAN)
		close(can_fd);

	printf("\n");
	return 0;
}
