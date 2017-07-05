#include <stdint.h>
#include <stdio.h>
#include <string.h>


#include "nb/config.h"
#include "nb/system.h"

int main(int argc, char *argv[])
{
    nb_config_modbus_tcp_t config;

    config.active = NB_CONFIG_MODBUS_TCP_ENABLE;
    config.coil_cmd = NB_CONFIG_MODBUS_WRITE_CMD_SINGLE;
    config.reg_cmd = NB_CONFIG_MODBUS_WRITE_CMD_SINGLE;
    config.timeout = 900;
    config.retries = 3;
    config.extra_delay = 100;

    if (nb_config_modbus_tcp_set(config) != 0) {
        puts("error establishing settings \n");
        return 1;
    }


    if (nb_system_reboot() != 0) {
        puts("error reboot command \n");
        return 1;
    }
    return 0;
}
