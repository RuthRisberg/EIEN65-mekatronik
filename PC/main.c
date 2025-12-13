/*Example serial port use in PC*/

/*Bridge the RXD and TXD pins in the serial port connector (or use your boards to perform a loop-back test!)*/
/*Compile the code, together with serialport.c ->  gcc -o myfile.exe serialexample.c serialport.c */
/*Execute your file!   ./myfile.exe */

#include "serialport.h"
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>

#define MSG_LEN

int main(void)
{
	/*Declaration of variables*/
	int sp,sl;
	int command, data;
	char msg_to_avr[MSG_LEN], msg_from_avr[MSG_LEN];
	
	/*Initialise serial port */
	sp = serial_init("/dev/ttyS0",0);
	if(sp == 0)
	{
		printf("Error! Serial port could not be opened.\n");
	}
	else
	{
		printf("Serial port open with identifier %d \n",sp);
	}

	while (1)
	{
	
		printf("Enter command: ");
		scanf("%d", &command);
		printf("\n");

		if (command == -1)
			break;

		printf("Enter data: ");		
		printf("Output: %s, Input: %s \n", cout, cin);
		printf("\n");

		write(sp,msg_to_avr,MSG_LEN);
		
		sleep(1);
		
		read(sp,msg_from_avr,MSG_LEN);
		
		printf("Answer: %d %d %d\n", msg_from_avr[0], msg_from_avr[1], msg_from_avr[2]);
	}
	
	serial_cleanup();

	return 0;
}
