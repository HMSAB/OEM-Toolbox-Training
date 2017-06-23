#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
//#include "uuid.h"


#ifndef __arm__
  #define ADDR_FILE "/sys/class/net/eno1/address"
#else
  #define ADDR_FILE "/sys/class/net/eth0/address"
#endif




int main(void){
  printf("test\n");
  char mac[18];
  char *hash;
  
  if( get_mac_addr(mac, ADDR_FILE) ){
    perror("Mac read error\n");
    return 1;
  }

  hash = gen_hash_string(mac);
  printf("%s\n", hash);
  free(hash);
  return 0;
}