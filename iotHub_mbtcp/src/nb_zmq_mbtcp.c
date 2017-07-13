#include "message.h"
#include "parson.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <uuid/uuid.h>
#include <zmq.h>

#define DEBUG (1)
#define DEV_TEST

#define SLAVE_ID 1
#define GUID_SIZE 37

static char target_ipaddr[16];
static int target_port;


static int message_parse(void *requester, void (*cbf)(uint16_t val));
static int zmq_request_send(char *message , void (*cbf)(uint16_t val) );



int init_ip_port(const char *ip, int port)
{
    if (port > 5000 || port < 0) {
        printf("port number invalid\n");
        return -1;
    }
    if (strlen(ip) < 6 || strlen(ip) > 36) {
        printf("ip address invalid\n");
        return -1;
    }
    target_port = port;
    strcpy(target_ipaddr, ip);
    return 0;
}


static void gen_uuid(char *guid)
{
    uuid_t uuid;
    uuid_generate( uuid );
    uuid_unparse(uuid, guid);
}


int get_u16_register(int reg, void (*cbf)(uint16_t val) )
{
    char *s = NULL;
    char uuid[GUID_SIZE];
    int res; 
    
    JSON_Status stat;

    JSON_Value *root_value = json_value_init_object();
    JSON_Object *root_object = json_value_get_object(root_value);
    
    JSON_Value *address_value = json_value_init_object();
    JSON_Object *address_object = json_value_get_object(address_value);

    JSON_Value *payload_value = json_value_init_object();
    JSON_Object *payload_object = json_value_get_object(payload_value);

    JSON_Value *payload_data_object_value = json_value_init_object();
    JSON_Object *payload_data_object = json_value_get_object(payload_data_object_value);

    JSON_Value *payload_data_array_value = json_value_init_array();
    JSON_Array *payload_data_array = json_value_get_array(payload_data_array_value);

    gen_uuid(uuid);
    //Address
    json_object_set_string(root_object, "id", uuid );
    json_object_set_string(address_object, "function", "Holding");
    json_object_set_number(address_object, "port", target_port);
    json_object_set_number(address_object, "slave_id", 1);
    json_object_set_number(address_object, "register", reg);
    json_object_set_string(address_object, "ip_address", target_ipaddr);

    // build payload data array
    json_object_set_string(payload_data_object, "command", "read");
    stat = json_object_set_value(payload_data_object, "address", address_value);
    

    stat = json_object_dotset_string(payload_data_object, "value.value_type","U16");

     // appended to the array 
    stat = json_array_append_value(payload_data_array, payload_data_object_value);
    json_object_set_string(payload_object, "payload_type", "request");
    stat = json_object_dotset_value(payload_object, "payload_data", payload_data_array_value);
    stat = json_object_dotset_value(root_object, "payload", payload_value);
    
    s = json_serialize_to_string_pretty(root_value);
#if(DEBUG)
    puts(s);
#endif
    
    json_value_free(root_value);
    res = zmq_request_send(s , cbf);
   
    json_free_serialized_string(s);
    
    return res;
}

static int zmq_request_send(char *message , void (*cbf)(uint16_t val) ){
    int res;
    void *context = zmq_ctx_new();
    void *requester = zmq_socket(context, ZMQ_REQ);

    zmq_connect(requester, "ipc:///tmp/zeromq/modbus_tcp");
    res = zmq_send(requester, message, strlen(message), 0);
    
   res = message_parse(requester , cbf);
    return res;
}

static int message_parse(void *requester, void (*cbf)(uint16_t val)){
    char buffer[600];
    JSON_Value *root_value;
    JSON_Object *root_object;
    JSON_Object *payload_object;
    JSON_Array *payload_array;
    JSON_Object *resp_object;
    int sizerec = zmq_recv(requester, buffer, 600, 0);
    buffer[sizerec] = '\0';

#if(DEBUG)
    puts(buffer);
    puts("\n");

#endif
    root_value = json_parse_string(buffer);
    if (json_value_get_type(root_value) != JSONObject) {
        puts("improper parse expected root to be object\n");
        return 1;
    }

    root_object = json_value_get_object(root_value);
    payload_object = json_object_get_object(root_object, "payload");
    printf("%s\n", json_object_get_string(payload_object,"payload_type"));

    payload_array = json_object_dotget_array(root_object,"payload.payload_data" );

    resp_object = json_array_get_object(payload_array, 0);
    if(resp_object != NULL){
        double val =0 ;
#if(DEBUG)
        printf("command response:%s \n",json_object_get_string(resp_object , "command") );
        printf("address function:%s\n",json_object_dotget_string(resp_object , "address.function") );
#endif
        if (json_object_dothas_value(resp_object,"value.value_data")){
            val = json_object_dotget_number(resp_object , "value.value_data");
            cbf((uint16_t)val);
#if(DEBUG)
            printf("%f\n",val);
#endif
        json_value_free(root_value);
          return 0 ;
          
        }
    }
    json_value_free(root_value);
    return 1;
}
