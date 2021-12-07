//#ifndef _SERIAL_H
//#define _SERIAL_H
struct dcb{
int open;
int events;
int dcb_status;

unsigned char* in;
int in_x;
int in_s;
unsigned char* out;
int out_x;
int out_s;

unsigned char ring[16];
int ring_inx;
int ring_outx;
int ring_s;

};
void top_handler();
int com_open(int baud_rate);
void set_int();
void input_h();
int com_write();
int com_read();
//#endif
