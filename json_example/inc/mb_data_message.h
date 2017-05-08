#ifndef MB_DATA_MESSAGE_H
#define MB_DATA_MESSAGE_H

#include <uuid/uuid.h>
#include <string>
#include <cstdint>
#include "message.h"

class mb_data_message {
private:
  uuid_t guid;
  char guid_string[GUID_SIZE];
  int reg;
  std::string function;
  std::string type;

  void gen_uuid(void) {
    uuid_generate(this->guid);
    uuid_unparse(guid, guid_string);
    // std::cout << guid_string << std::endl;
  }

public:
  void set_register(int reg) { this->reg = reg; }
  uint16_t request(int reg, const char *command, const char *type,
                   const char *function, int slaveid, int port, const char *ip);
};

#endif