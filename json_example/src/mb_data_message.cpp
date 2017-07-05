#include "mb_data_message.h"
#include <cstring>
#include <iostream>
#include <jansson.h>
#include <stdint.h>
#include <stdlib.h>
#include <string>
#include <uuid/uuid.h>
#include <zmq.h>


static void message_rec(void *requester);

void mb_data_message::get_register(int reg, std::string command, std::string type, std::string function)
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

    void *context = zmq_ctx_new();
    void *requester = zmq_socket(context, ZMQ_REQ);

    zmq_connect(requester, "ipc:///tmp/zeromq/modbus_tcp");
    zmq_send(requester, s, strlen(s), 0);

    message_rec(requester);

    free(s);
}

static void message_rec(void *requester)
{
    json_t *jroot;
    json_error_t jerror;
    char buffer[600];
    int sizerec = zmq_recv(requester, buffer, 600, 0);

    buffer[sizerec] = '\0';
    puts(buffer);

    jroot = json_loads(buffer, 0, &jerror);
    if (!jroot) {
        puts("error with json response parsing: json loads\n");
        return;
    }

    if (!json_is_object(jroot)) {
        puts("error with json response parsing: json is objects\n");
        json_decref(jroot);
        return;
    }

    {
        json_t *id, *payload;
        id = json_object_get(jroot, "id");
        if (!json_is_string(id)) {
            puts("error with json response parsing: json id\n");
            json_decref(jroot);
            return;
        }
        printf("ID value: %s\n ", (char*)id);

        payload = json_object_get(jroot, "payload");
         if (!json_is_object(payload)) {
            puts("error with json response parsing: json id\n");
            json_decref(jroot);
            return;
        }
    
    }
}

mb_data_message::mb_data_message(int slaveid, int port, std::string ip)
{
    this->slaveid = slaveid;
    this->port = port;
    this->ip = ip;
}