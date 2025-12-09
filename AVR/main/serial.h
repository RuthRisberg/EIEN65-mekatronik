#ifndef eien65_serial_h
#define eien65_serial_h

void init_serial();
void send(char header, char payload);
int receive(char* header, char* payload); // returns 1 if message received

#endif
