#include "message.h"
#include <jansson.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <uuid/uuid.h>
#include <zmq.h>

#define DEBUG

 
static char target_ipaddr[12];
static int target_port


int set_ip_port(const char *ip, int port)
{
    if (port > 5000 || port < 0) {
        printf("port number invalid\n");
        return -1
    }

    if (strlen(ip) < 6 || strlen(ip) > 36) {
        printf("ip address invalid\n");
        return -1
    }

    target_port = pprt;
    strcpy(target_ipaddr, ip);
    return 0;
}


static void gen_uuid(uuit_t *guid)
{
    uuid_generate( guid );
}


uint16_t
    get_u16_register(int reg)
{
    char *s = NULL;

    json_t *root = json_object();
    json_t *payload = json_object();
    json_t *payload_data = json_array();

    json_t *command_json = json_object();
    json_t *address_json = json_object();
    json_t *value_json = json_object();

    json_object_set_new(command_json, "command", json_string("read");
    json_object_set_new(address_json, "function", json_string("input"));
    json_object_set_new(address_json, "slave_id", json_integer(slaveid));
    json_object_set_new(address_json, "register", json_integer(reg));
    json_object_set_new(address_json, "ip_address", json_string(ip.c_str()));
    json_object_set_new(address_json, "ip_port", json_integer(port));

    json_object_set_new(value_json, "value_type", json_string(type.c_str()));
    if (strncmp(COMMAND[write], command.c_str(), 5) == 0) {
        json_object_set_new(value_json, "value_data", json_integer(val));
    }
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

#ifdef DEBUG
    puts("\n\n\n********  Request Message *********");
    puts(s);
    puts("\n");
#endif
    json_decref(root);
    json_decref(payload);
    json_decref(payload_data);

    // void *context = zmq_ctx_new();
    // void *requester = zmq_socket(context, ZMQ_REQ);

    // zmq_connect(requester, "ipc:///tmp/zeromq/modbus_tcp");
    // zmq_send(requester, s, strlen(s), 0);

    ///message_start_parse(requester, this->callback);

    free(s);
}