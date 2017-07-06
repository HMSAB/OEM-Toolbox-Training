#include "mb_data_message.h"
#include <iostream>
#include <jansson.h>
#include <stdlib.h>
#include <string>
#include <uuid/uuid.h>
#include "message.h"

int main(int argc, char *argv[])
{
  printf("Running program with JANSSON VERSION %d.%d.%d\n", JANSSON_MAJOR_VERSION,
         JANSSON_MINOR_VERSION, JANSSON_MICRO_VERSION);

  mb_data_message my_message(1, MBTCP_PORT, "192.168.1.9");


  my_message.write_u16_register(2, 56);

  my_message.read_u16_register(2);
  // my_message.read_u16_register(2);
  // my_message.read_u16_register(3);
  // my_message.read_u16_register(4);

  std::cout << "end" << std::endl;
  return 0;
}
