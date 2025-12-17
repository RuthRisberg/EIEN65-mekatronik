#ifndef eien65_serial_h
#define eien65_serial_h

void init_serial();
void send(unsigned char header, unsigned char payload);
int receive(unsigned char* header, unsigned char* payload); // returns 1 if message received
void isr_receive_serial();

#endif
