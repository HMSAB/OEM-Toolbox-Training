#include <iostream>
#include <stdlib.h>
#include <jansson.h>
#include <string>
#include <uuid/uuid.h>
#include <stdint.h> 
#include "mb_data_message.h"

void mb_data_message::request(int reg, const char *command, const char *type, const char *function, int slaveid , int port , const char * ip  ) 
{
    this->reg = reg; 
    this->type = type; 
    this->function = function;
    char* s = NULL;

    json_t *root = json_object();
    json_t *payload = json_object();
    json_t *payload_data = json_array();

    json_t *command_json = json_object();
    json_t *address_json = json_object();
    json_t *value_json = json_object();

    json_object_set_new( command_json, "command", json_string(command) );
    json_object_set_new( address_json, "function", json_string(function) );
    json_object_set_new( address_json, "slave_id", json_integer(slaveid) );
    json_object_set_new( address_json, "register", json_integer(reg) );
    json_object_set_new( address_json, "ip_address", json_string(ip) );
    json_object_set_new( address_json, "ip_port", json_integer(port) );

    json_object_set_new( value_json, "value_type", json_string(type) );
    json_object_set_new( command_json, "address", address_json );
    json_object_set_new( command_json, "value", value_json );
    json_array_append(payload_data, command_json);
    
    gen_uuid();
    json_object_set_new( root, "id", json_string(this->guid_string) );
    json_object_set_new( payload, "payload_type", json_string("request") );

    json_object_set_new( root, "payload", payload );
    json_object_set_new( payload, "payload_data", payload_data );


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
