#ifndef NB_ZMQ_H
#define NB_ZMQ_H

int init_ip_port(const char *ip, int port);

int get_u16_register(int reg, void (*cbf)(uint16_t val) );

#endif