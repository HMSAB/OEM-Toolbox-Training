#include "mb_data_message.h"
#include <cstring>
#include <iostream>
#include <jansson.h>
#include <stdint.h>
#include <stdlib.h>
#include <string>
#include <uuid/uuid.h>
#include <zmq.h>


static void message_start_parse(void *requester);
static char *parse_payload(json_t *payload);
static char *parse_array(json_t *payload_data);
static char *parse_object(json_t *payload_data);

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

    message_start_parse(requester);

    free(s);
}

static void message_start_parse(void *requester)
{
    json_t *jroot, *id, *payload;
    json_error_t jerror;
    char buffer[600];
    char *err;
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
    id = json_object_get(jroot, "id");
    if (!json_is_string(id)) {
        puts("error with json response parsing: json id\n");
        json_decref(jroot);
        return;
    }
    printf("ID value: %s\n", json_string_value(id));
    payload = json_object_get(jroot, "payload");
    if (!json_is_object(payload)) {
        puts("error with json response parsing: json id\n");
        json_decref(jroot);
        return;
    }
    err = parse_payload(payload);

    if (err != NULL) {
        printf("%s\n", err);
        free(err);
    }
}

static char *parse_object(json_t *payload_data)
{
    const char *key;
    json_t *obj_val;
    json_object_foreach(payload_data, key, obj_val){
        switch json_typeof(obj_val){
            case JSON_OBJECT:
                    return parse_object(obj_val);
                break;
            case JSON_ARRAY:
                return parse_array(obj_val);
            break;
            case JSON_STRING:
                    printf("json string %s %s \n",key, json_string_value(obj_val) );
                    return NULL;
            break;
            case JSON_INTEGER:
                   printf("json int %s %d \n",key, json_integer_value(obj_val) );
                   return NULL;
            break;
            case JSON_REAL:
                 printf("json real %s %f \n", key, json_integer_value(obj_val));
            break;
            default:
            printf("not able to handle object  %s \n",key);
            break;
        }                  
    }
    
}

static char *parse_array(json_t *payload_data)
{
   
    size_t index;
    json_t *array_value;
    json_array_foreach(payload_data, index, array_value){
        switch json_typeof(array_value){
            case JSON_OBJECT:
                return parse_object(array_value);
            break;
            case JSON_ARRAY:
                return parse_array(array_value);
            break;
            // case JSON_STRING:
            //         printf("json string %s %s \n",key, json_string_value(array_value) );
            // break;
            // case JSON_INTEGER:
                
            // break;
            // case JSON_REAL:
                
            // break;
            default:
            printf("not able to handle index %d  type %d\n", index, json_typeof(array_value));
            break;

        }
    }
    return NULL;
}

static char *parse_payload(json_t *payload)
{

    json_t *payload_type, *payload_data_array;
    payload_type = json_object_get(payload, "payload_type");
    if (!json_is_string(payload_type)) {
        const char *err = "paylod_type was not found to be string\n";
        json_decref(payload);
        return strdup(err);
    }

    payload_data_array = json_object_get(payload, "payload_data");
    if (!json_is_array(payload_data_array)) {
        const char *err = "paylod_type was not found to be string\n";
        json_decref(payload);
        return strdup(err);
    }
    return parse_array( payload_data_array);
}

mb_data_message::mb_data_message(int slaveid, int port, std::string ip)
{
    this->slaveid = slaveid;
    this->port = port;
    this->ip = ip;
}




    // json_array_foreach(payload_data_array, index, array_value){
    //     switch json_typeof(array_value)
    //     {
    //         case JSON_OBJECT:
    //             json_object_foreach(array_value, key, obj_val){
    //                 switch json_typeof(obj_val){
    //                     case JSON_OBJECT:
    //                         json_object_foreach(array_value, key, obj_val){
    //                             switch json_typeof(obj_val){
    //                                 case JSON_STRING:
    //                                     printf("json string %s %s \n",key, json_string_value(obj_val) );
    //                                 break;
    //                                 case JSON_INTEGER:
    //                                     printf("json int %s %d \n", key, obj_val);
    //                                 break;
    //                                 default:
    //                                 break;
    //                             }
    //                         }
    //                     break;

    //                     case JSON_STRING:
    //                         printf("json string %s %s \n",key, json_string_value(obj_val) );
    //                     break;
    //                     case JSON_INTEGER:
    //                         printf("json int %s %d \n", key, obj_val);
    //                     break;
    //                     case JSON_REAL:
    //                     break;

    //                     default:
    //                         printf("not able to handle key %s , val  %s \n", key, obj_val);
    //                     break;        
    //                 }
    //             }
    //         break;
    //         case JSON_INTEGER:
    //         break;
    //         case JSON_REAL:
    //         break;
    //         case JSON_STRING:
    //             printf("json string %s\n",json_string_value(array_value) );
    //         break;
    //         default:
    //         printf("not able to handle index %d  type %d \n", index, json_typeof(array_value));
    //     }
    // }




    // {
    //     json_t *obj = json_array_get(payload_data, 1);
    //     if (obj != NULL) {
    //         printf("type of obj %d\n", json_typeof(obj));
    //         if (!json_is_object(obj)) {
    //             const char *err = "value was not found to be and object \n";
    //             json_decref(payload_data);
    //             return strdup(err);
    //         }
    //     }

    // // }