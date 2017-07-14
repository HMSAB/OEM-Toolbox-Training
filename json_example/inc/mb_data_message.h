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
  void (*callback)(uint16_t val);

  void gen_uuid(void)
  {
    uuid_generate(this->guid);
    uuid_unparse(guid, guid_string);
  }
  void reg_func(int reg, std::string command, std::string type, std::string function , uint16_t val);

public:
  void read_u16_register(int reg);
  void read_u16_register_callback(int reg, void (*resp_cbf)(uint16_t val)  );
  void write_u16_register(int reg, uint16_t val);
  void read_flt_register(int reg);
  void init_zmq(void);
  void close_zmq(void);
  /*constructor*/
  mb_data_message(int slaveid, int port, std::string ip);
};

#endif