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
#include <threads.h>
#include <assert.h>

#include "shared_enums.h"
#include "enum_to_str.h"

#define MSG_LEN 3

static volatile int stop = 0;

int read_thread_func(void* serial)
{
	unsigned char msg_from_avr[MSG_LEN];
	int header, data, checksum, index;
	while (!stop)
	{
		index = 0;
		while (index != 3)
		{
			while (!read(*((int*)serial),&msg_from_avr[index],1)); // pretty sure this blocks until 3 bytes arrive
				thrd_sleep(&(struct timespec){.tv_nsec=100000000}, NULL); // wait 0.1 second until trying again
			index++;
			thrd_sleep(&(struct timespec){.tv_nsec=10000000}, NULL); // wait 10 ms
		}

		header = msg_from_avr[0];
		data = msg_from_avr[1];
		checksum = msg_from_avr[2];

		if ((header ^ data) != checksum)
			printf("\rReceived broken answer: %d %d %d\n", header, data, checksum);
		else if (header == ERROR)
			printf("\rReceived: (%d)ERROR (%d)%s\n", header, data, get_error_msg(data));
		else
			printf("\rReceived: (%d)%s %d\n", header, get_avr_to_pc_header(header), data);
	}
	return 0;
}

int main(void)
{
	/*Declaration of variables*/
	int sp;
    thrd_t read_thread;
    int temp, header, data;
	char buf[20];
	int index = 0;
	
	/*Initialise serial port */
	sp = serial_init("/dev/ttyUSB0",0);
	if(sp == 0)
	{
		printf("Error! Serial port could not be opened.\n");
	}
	else
	{
		printf("Serial port open with identifier %d \n",sp);
	}

	// receive messages from serial (write to stdout)
    temp = thrd_create(&read_thread, read_thread_func, &sp);
	if (!(temp==thrd_success))
		printf("%d\n", temp);
	assert(temp == thrd_success);

	// send messages to serial (read from stdin)
	while (!stop)
	{
		buf[index] = getchar();
		index++;
		if (buf[index-1] == ' ')
		{
			header = atoi(buf);
			index = 0;
		}
		else if (buf[index-1] == '\n')
		{
			data = atoi(buf);
			index = 0;
			if (header >= 0 && header < 256)
			{
				printf("\rSending: (%d)%s %d\n", header, get_pc_to_avr_header(header), data);
				buf[0] = header;
				buf[1] = data;
				buf[2] = buf[0] ^ buf[1];
				write(sp,buf,3);
				printf("Enter new command: ");
			}
			else if (header == -1)
			{
				printf("Stopping after next received message\n");
				header = ECHO;
				data = 0;
				printf("\rSending: (%d)%s %d\n", header, get_pc_to_avr_header(header), data);
				buf[0] = header;
				buf[1] = data;
				buf[2] = buf[0] ^ buf[1];
				write(sp,buf,3);
				stop = 1;
				thrd_join(read_thread, &temp);
				printf("Result of stopping reader thread: %d\n", temp);
			}
			else
			{
				printf("Invalid command.\n");
			}
		}
		else if (index == 20)
		{
			printf("\rRead buffer overflow. Resetting buffer.\n");
		}
	}

	serial_cleanup();

	return 0;
}
