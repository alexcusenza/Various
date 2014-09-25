/*
 * GPIO Toggle Routine
 *
 *  Created on: Dec 6, 2013
 *      Author: alexcusenza
 */

#include <stdio.h>
#include <string.h>
//#include <stdlib.h>
//#include <sys/io.h>
//#include <fcntl.h>



/***********************
 * DEFINE CONSTANTS
 ************************/
#define MAX 64
#define TRUE 1
#define FALSE 0

/**************************
 * DEFINE GLOBAL VARIABLES
 **************************/


/***********************
 * FUNCTION DECLARATION
 ***********************/
int flashGPIO(int);


/***********************
 * FUNCTION ROUTINES
 ***********************/
int flashGPIO(int GPIOPin)
{
	FILE *GPIOHandle;
	char setValue[5];
	char GPIOString[5], GPIODirection[MAX], GPIOValue[MAX], GPIOExport[MAX], GPIOUnexport[MAX];
	char GPIOout[5];
	int toggle = TRUE;
	int i = 0;
	//char out = "out";

	/** Define the Pins, Direction Value */
	printf("GPIO Pin: %d \n",GPIOPin);

	sprintf(GPIOString, "%d", GPIOPin);
	sprintf(GPIOExport, "/sys/class/gpio/export");
	sprintf(GPIOUnexport, "/sys/class/gpio/unexport");
	sprintf(GPIODirection, "/sys/class/gpio/gpio%d/direction", GPIOPin);
	sprintf(GPIOValue, "/sys/class/gpio/gpio%d/value", GPIOPin);
	sprintf(GPIOout, "out");

	// *** EXPORTING THE PIN ***
	GPIOHandle = fopen(GPIOExport, "ab");
	if (GPIOHandle == NULL)
	{
		printf("Cannot open export GPIO pin %d.\n", i);
		return 1;
	}
	else
	{
		//sprintf(GPIOString, "%d", i);
		strcpy(setValue, GPIOString);
		fwrite(&setValue, sizeof(char), sizeof(setValue), GPIOHandle);
		fclose(GPIOHandle);
	}


	// *** PIN DIRECTION ***
	GPIOHandle = fopen(GPIODirection, "rb+");
	if (GPIOHandle == NULL)
	{
		printf("Cannot open direction handle.\n");
		return 1;
	}
	strcpy(setValue, "in");
	fwrite(&setValue, sizeof(char), sizeof(setValue), GPIOHandle);
	fclose(GPIOHandle);



	// +--------------------------+
	// RUN Output or Input Routine
	// +--------------------------+


	if( strcmp( setValue, GPIOout) == 0 )
	{
		printf("outputting\n");

		for(i=0; i<4; i++)
		{
			GPIOHandle = fopen(GPIOValue, "rb+");
			if (GPIOHandle == NULL)
			{
				printf("Cannot open value handle.\n");
				return 1;
			}

			if (toggle)
			{
				toggle = FALSE;
				strcpy(setValue, "1");
				printf("%2d ON ",i);
			}
			else
			{
				toggle = TRUE;
				strcpy(setValue, "0");
				printf("%2d OFF ",i);
			}

			printf("\n");
			fwrite(&setValue, sizeof(char), sizeof(setValue), GPIOHandle);
			fclose(GPIOHandle);
			usleep(250000);
		}
	}
	else
	{
		printf("inputting\n");

		for(i=0; i<1; i++)
		{
			GPIOHandle = fopen(GPIOValue, "rb+");
			if (GPIOHandle == NULL)
			{
				printf("Cannot open value handle.\n");
				return 1;
			}

			//printf("attempt #%d:  ",i);
			fread(&setValue, sizeof(char), 2, GPIOHandle );
			printf("input value = %s", setValue);
			printf("\n");

			fclose(GPIOHandle);
			usleep(500000);
		}
	}


	/*
	 * Unexport the pin
	 */
	GPIOHandle = fopen(GPIOUnexport, "ab");
	if (GPIOHandle == NULL)
	{
		printf("Cannot unexport GPIO pin.\n");
		//return 1;
	}
	else
	{
		//sprintf(GPIOString, "%d", i);
		strcpy(setValue, GPIOString);
		fwrite(&setValue, sizeof(char), sizeof(setValue), GPIOHandle);
		fclose(GPIOHandle);
	}

	return 0;

}

/*********************
 * MAIN ROUTINE
 *********************/
int main(void)
{
	flashGPIO(207);
	//flashGPIO(208);
	flashGPIO(212);
	flashGPIO(213);
	flashGPIO(214);
	flashGPIO(216);

	flashGPIO(220);
	flashGPIO(223);
	flashGPIO(224);
	flashGPIO(226);
	flashGPIO(228);
	flashGPIO(230);
	flashGPIO(233);
	flashGPIO(234);
	flashGPIO(242);
	flashGPIO(243);
	flashGPIO(244);
	flashGPIO(245);
	flashGPIO(254);

	return 0;
}




