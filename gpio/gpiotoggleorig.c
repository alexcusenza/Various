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

/***********************
 * DEFINE GLOBAL VARIABLES
 ************************/


/***********************
 * FUNCTION DECLARATION
 ************************/
int flashGPIO(int);


/***********************
 * FUNCTION ROUTINES
 ************************/
int flashGPIO(int GPIOPin)
{
	FILE *GPIOHandle;
	char setValue[5];
	char GPIOString[5], GPIODirection[MAX], GPIOValue[MAX], GPIOExport[MAX], GPIOUnexport[MAX];
	int toggle = TRUE;
	int i = 0;

	/** Define the Pins, Direction Value */
	printf("GPIO Pin: %d \n",GPIOPin);

	sprintf(GPIOString, "%d", GPIOPin);
	sprintf(GPIOExport, "/sys/class/gpio/export");
	sprintf(GPIOUnexport, "/sys/class/gpio/unexport");
	sprintf(GPIODirection, "/sys/class/gpio/gpio%d/direction", GPIOPin);
	sprintf(GPIOValue, "/sys/class/gpio/gpio%d/value", GPIOPin);


	/** Export the pin */
	GPIOHandle = fopen(GPIOExport, "ab");
	if (GPIOHandle == NULL)
	{
		printf("Cannot open export GPIO pin.\n");
		return 1;
	}

	strcpy(setValue, GPIOString);
	fwrite(&setValue, sizeof(char), sizeof(setValue), GPIOHandle);
	fclose(GPIOHandle);

	/** Set direction of pin */
	GPIOHandle = fopen(GPIODirection, "rb+");
	if (GPIOHandle == NULL)
	{
		printf("Cannot open direction handle.\n");
		return 1;
	}
	strcpy(setValue, "out");
	fwrite(&setValue, sizeof(char), 3, GPIOHandle);
	fclose(GPIOHandle);

	/** Set output value */
	for(i=0; i<9; i++)
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
		}
		else
		{
			toggle = TRUE;
			strcpy(setValue, "0");
		}

		fwrite(setValue, sizeof(char), sizeof(setValue), GPIOHandle);
		fclose(GPIOHandle);
		sleep(1);
	}

	/** Unexport the pin */
	GPIOHandle = fopen(GPIOUnexport, "ab");
	if (GPIOHandle == NULL)
	{
		printf("Cannot unexport GPIO pin.\n");
		return 1;
	}

	strcpy(setValue, GPIOString);
	fwrite(&setValue, sizeof(char), sizeof(setValue), GPIOHandle);
	fclose(GPIOHandle);

	return 0;

}

/*********************
 * MAIN ROUTINE
 **********************/
int main(void)
{
	flashGPIO(212);
	return 0;
}




