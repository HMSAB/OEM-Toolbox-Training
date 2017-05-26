#include "mb_data_message.h"
#include <iostream>
#include <jansson.h>
#include <stdint.h>
#include <stdlib.h>
#include <string>
#include <uuid/uuid.h>


void mb_data_message::request(int reg, std::string command, std::string type, std::string function)
{
    char *s = NULL;

    json_t *root = json_object();
    json_t *payload = json_object();
    json_t *payload_data = json_array();

    json_t *command_json = json_object();
    json_t *address_json = json_object();
    json_t *value_json = json_object();

    json_object_set_new(command_json, "command", json_string(command.c_str()));
    json_object_set_new(address_json, "function", json_string(function.c_str()));
    json_object_set_new(address_json, "slave_id", json_integer(slaveid));
    json_object_set_new(address_json, "register", json_integer(reg));
    json_object_set_new(address_json, "ip_address", json_string(ip.c_str()));
    json_object_set_new(address_json, "ip_port", json_integer(port));

    json_object_set_new(value_json, "value_type", json_string(type.c_str()));
    json_object_set_new(command_json, "address", address_json);
    json_object_set_new(command_json, "value", value_json);
    json_array_append(payload_data, command_json);

    gen_uuid();
    json_object_set_new(root, "id", json_string(this->guid_string));
    json_object_set_new(payload, "payload_type", json_string("request"));

    json_object_set_new(root, "payload", payload);
    json_object_set_new(payload, "payload_data", payload_data);


#ifdef DEV_TEST
    s = json_dumps(root, JSON_INDENT(2));
#else
    s = json_dumps(root, 0);
#endif

    puts(s);
    json_decref(root);
    json_decref(payload);
    json_decref(payload_data);

    free(s);
}


mb_data_message::mb_data_message( int slaveid, int port, std::string ip)
{
      this->slaveid = slaveid;
      this->port = port;
      this->ip = ip;
}