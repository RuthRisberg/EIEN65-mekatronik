#include <avr/io.h>
#include "serial.h"
#include "globals.h"

#define F_OSC 1000000
#define BAUD 9600
#define MYUBRR FOSC/16/BAUD-1

static int inited = 0;

void init_serial()
{
    UBRR0H = (unsigned char)(MYUBRR >> 8);
    UBRR0L = (unsigned char)MYUBRR;
    UCSR0B |= (1<<RXEN0) | (1<<TXEN0);
    UCSR0C = (1 << USCZ00) | (1 << USCZ01); // async, no parity, 1 stop bit, 8 bit data
    inited = 1;
}

void send_byte(char data)
{
    /* Wait for empty transmit buffer */
    while ( !( UCSR0A & (1<<UDRE0)) );
    /* Put data into buffer, sends the data */
    UDR0 = data;
}

void send(char header, char payload)
{
    if (!inited)
        error(UNINITIALIZED);
    send_byte(header);
    send_byte(payload);
    send_byte(header ^ payload); // parity
}

static char received_header = 0;
static char received_payload = 0;
static int has_received = 0;
int receive(char* header, char* payload) // returns 1 if message received
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

char receive_byte()
{
    while(!(UCSR0A & (1 << RXC0)));
    return UDR0;
}
ISR(USART_RX_vect)
{
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
        if (received_header ^ received_payload == receive_byte()) // parity correct
        {
            has_received = 1;
        }
        else
        {
            error(INCORRECT_PARITY);
        }
    }
}

void error(error_msg msg)
{
    send(ERROR, msg);
}
