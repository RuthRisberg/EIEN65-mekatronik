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
	int header, data, checksum, high_part;
	while (!stop)
	{
		read(*((int*)serial),&msg_from_avr[0],MSG_LEN); // blocks until a full message is received

		header = msg_from_avr[0];
		data = msg_from_avr[1];
		checksum = msg_from_avr[2];

		if ((header ^ data) != checksum)
			printf("\rReceived broken answer: %d %d %d\n", header, data, checksum);
		else if (header == ERROR)
			printf("\rReceived: (%d)ERROR (%d)%s\n", header, data, get_error_msg(data));
		else
		{
			char* h_str = get_avr_to_pc_header(header);
			size_t h_len = strlen(h_str);
			if (h_str[h_len-2] == '_' && h_str[h_len-1] == 'H')
			{
				// printf("\rReceived: %d\n", header);
				high_part = data;
			}
			else if (h_str[h_len-2] == '_' && h_str[h_len-1] == 'L')
			{
				if ((high_part<<8)+data > 30000)
					printf("\rReceived: (%d)%s %d\n", header, h_str, ((high_part<<8)+data)-65536);
				else
					printf("\rReceived: (%d)%s %d\n", header, h_str, (high_part<<8)+data);
			}
			else
			{
				printf("\rReceived: (%d)%s %d\n", header, h_str, data);
			}
		}
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
	sp = serial_init("/dev/ttyUSB0", 0, MSG_LEN);
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
