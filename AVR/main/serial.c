#include "globals.h"
#include <avr/io.h>
#include <util/delay.h>
#include "serial.h"
#include "leds.h"
#include "sensors.h"

#define BAUD 2400
//#define MYUBRR (F_CPU/16/BAUD-1)
#define MYUBRR 25

static uint8_t inited = 0;

void init_serial()
{
	UBRR0 = MYUBRR;
    UCSR0B |= (1<<RXEN0) | (1<<TXEN0) | (1<<RXCIE0);
    // UCSR0C =// async, no parity, 1 stop bit, 8 bit data by default
    inited = 1;
}

void send_byte(uint8_t data)
{
    /* Wait for empty transmit buffer */
    while ( !( UCSR0A & (1<<UDRE0)) );
    /* Put data into buffer, sends the data */
    UDR0 = data;
}

void send(uint8_t header, uint8_t payload)
{
    if (!inited)
        error(UNINITIALIZED);
    send_byte(header);
    send_byte(payload);
    send_byte(header ^ payload); // parity
	_delay_ms(20);
}

static uint8_t received_header = 0;
static uint8_t received_payload = 0;
static int has_received = 0;
uint8_t receive(uint8_t* header, uint8_t* payload) // returns 1 if message received
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

uint8_t receive_byte()
{
    while(!(UCSR0A & (1 << RXC0)));
    return UDR0;
}
void isr_receive_serial()
{
	uint8_t parity;
	//uint8_t header = receive_byte();
	//uint8_t data = receive_byte();
	//uint8_t parity = receive_byte();
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
