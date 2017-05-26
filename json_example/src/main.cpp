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


  mb_data_message my_message( 99 ,MBTCP_PORT, "192.168.1.44" );
  my_message.set_register(4);
  my_message.request(4, COMMAND[read], VALUE_TYPE[U16], FUNCTION[Holding]);

  std::cout << "end" << std::endl;
  return 0;
}
