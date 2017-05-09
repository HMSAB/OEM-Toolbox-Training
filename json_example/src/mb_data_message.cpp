#include "mb_data_message.h"
#include <iostream>
#include <jansson.h>
#include <cstdint>
#include <stdlib.h>
#include <string>
#include <uuid/uuid.h>
#include <zmq.hpp>

#define MAX_REPLY_SIZE 512


uint16_t mb_data_message::zmq_request_parse(char *s) {
  json_t *root;
  json_error_t error; 

  root = json_loads(s, 0, &error);

  if(!root){
    std::cout << "not root" << std::endl;
    return -1;
  }

}


void mb_data_message::zmq_request_send(char *s) {
  zmq::context_t context(1);
  zmq::socket_t socket(context, ZMQ_REQ);

  std::cout << "Connecting to endpoint" << std::endl;
  socket.connect("ipc:///tmp/zeromq/modbus_tcp");

  zmq::message_t request(strlen(s));
  memcpy(request.data(), s, strlen(s));
  std::cout << "Sending request " << std::endl;
  socket.send(request);

  // Get the reply.
  zmq::message_t reply(MAX_REPLY_SIZE);
  socket.recv(&reply);
  std::cout << "Received response " << (char *)reply.data() << std::endl;
}

uint16_t mb_data_message::request(int reg, const char *command,
                                  const char *type, const char *function,
                                  int slaveid, int port, const char *ip) {
  this->reg = reg;
  this->type = type;
  this->function = function;
  char *s = NULL;

  json_t *root = json_object();
  json_t *payload = json_object();
  json_t *payload_data = json_array();

  json_t *command_json = json_object();
  json_t *address_json = json_object();
  json_t *value_json = json_object();

  json_object_set_new(command_json, "command", json_string(command));
  json_object_set_new(address_json, "function", json_string(function));
  json_object_set_new(address_json, "slave_id", json_integer(slaveid));
  json_object_set_new(address_json, "register", json_integer(reg));
  json_object_set_new(address_json, "ip_address", json_string(ip));
  json_object_set_new(address_json, "ip_port", json_integer(port));

  json_object_set_new(value_json, "value_type", json_string(type));
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
  zmq_request_send(s);
  std::cout << "return from function" << std::endl;

  json_decref(root);
  json_decref(payload);
  json_decref(payload_data);
  free(s);

  return 0;
}
