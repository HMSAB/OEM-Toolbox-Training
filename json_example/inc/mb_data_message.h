#ifndef MB_DATA_MESSAGE_H
#define MB_DATA_MESSAGE_H

#include <uuid/uuid.h>
#include <string>
#include "message.h"

class mb_data_message
{
private:
  uuid_t guid;
  char guid_string[GUID_SIZE];
  int reg, slaveid, port;
  std::string ip;

  void gen_uuid(void)
  {
    uuid_generate(this->guid);
    uuid_unparse(guid, guid_string);
    // std::cout << guid_string << std::endl;
  }
  void get_register(int reg, std::string command, std::string type, std::string function);

public:
  //void get_register(int reg, std::string command, std::string type, std::string function);
  void read_u16_register(int reg);
  /*constructor*/
  mb_data_message(int slaveid, int port, std::string ip);
};

#endif