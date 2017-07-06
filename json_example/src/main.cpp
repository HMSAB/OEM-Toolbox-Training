#include "mb_data_message.h"
#include <iostream>
#include <jansson.h>
#include <stdlib.h>
#include <string>
#include <uuid/uuid.h>
#include "message.h"

void printer(uint16_t val){
  printf("the return value is %X \n", val);
}

int main(int argc, char *argv[])
{
  printf("Running program with JANSSON VERSION %d.%d.%d\n", JANSSON_MAJOR_VERSION,\
         JANSSON_MINOR_VERSION, JANSSON_MICRO_VERSION);


 /* this will set the ip address, and port of the target device */ 
  mb_data_message my_message(1, MBTCP_PORT, "192.168.1.9");


  my_message.write_u16_register(2, 16);
  my_message.write_u16_register(1, 32);
  my_message.write_u16_register(4, 6.6);

  my_message.read_u16_register_callback(2, &printer);
  my_message.read_u16_register_callback(1, &printer);
  my_message.read_u16_register_callback(4, &printer);
  return 0;
}
