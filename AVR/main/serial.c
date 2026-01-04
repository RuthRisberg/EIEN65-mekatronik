#include "globals.h"
#include <avr/io.h>
#include <util/delay.h>
#include "serial.h"
#include "leds.h"
#include "sensors.h"

#define BAUD 2400
//#define MYUBRR (F_CPU/16/BAUD-1)
#define MYUBRR 25

static int inited = 0;

void init_serial()
{
    UBRR0H = (unsigned char)(MYUBRR >> 8);
    UBRR0L = (unsigned char)MYUBRR;
    UCSR0B |= (1<<RXEN0) | (1<<TXEN0) | (1<<RXCIE0);
    // UCSR0C =// async, no parity, 1 stop bit, 8 bit data by default
    inited = 1;
}

void send_byte(unsigned char data)
{
    /* Wait for empty transmit buffer */
    while ( !( UCSR0A & (1<<UDRE0)) );
    /* Put data into buffer, sends the data */
    UDR0 = data;
}

void send(unsigned char header, unsigned char payload)
{
    if (!inited)
        error(UNINITIALIZED);
    send_byte(header);
    send_byte(payload);
    send_byte(header ^ payload); // parity
	_delay_ms(50);
}

static unsigned char received_header = 0;
static unsigned char received_payload = 0;
static int has_received = 0;
int receive(unsigned char* header, unsigned char* payload) // returns 1 if message received
{
    if (has_received)
    {
        *header = received_header;
        *payload = received_payload;
        has_received = 0;
        return 1;
    }
    else
        return 0;
}

unsigned char receive_byte()
{
    while(!(UCSR0A & (1 << RXC0)));
    return UDR0;
}
void isr_receive_serial()
{
	unsigned char parity;
	//unsigned char header = receive_byte();
	//unsigned char data = receive_byte();
	//unsigned char parity = receive_byte();
	//send_byte(header);
	//send_byte(data);
	//send_byte(parity);
	if (has_received)
	{
		error(BUFFER_FULL);
		receive_byte();
		receive_byte();
		receive_byte();
	}
	else
	{
		received_header = receive_byte();
		received_payload = receive_byte();
		parity = receive_byte();
		if ((received_header ^ received_payload) == parity) // parity correct
		{
			has_received = 1;
		}
		else
		{
			error(INCORRECT_PARITY);
			send(RECEIVED_HEADER, received_header);
			send(RECEIVED_DATA, received_payload);
			send(RECEIVED_PARITY, parity);
		}
	}
}

void error(error_msg_t msg)
{
	turn_on_led(ERROR_LED);
    send(ERROR, msg);
}
