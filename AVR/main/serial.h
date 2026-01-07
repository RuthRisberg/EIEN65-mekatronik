#ifndef eien65_serial_h
#define eien65_serial_h

void init_serial();
void send(uint8_t header, uint8_t payload);
uint8_t receive(uint8_t* header, uint8_t* payload); // returns 1 if message received
void isr_receive_serial();

#endif
