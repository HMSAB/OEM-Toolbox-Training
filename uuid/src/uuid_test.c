#include <errno.h>
#include <openssl/md5.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <uuid/uuid.h>
#include "uuid.h"

#ifndef __arm__
#define ADDR_FILE "/sys/class/net/eno1/address"
#else
#define ADDR_FILE "/sys/class/net/eth0/address"
#endif
const char *spaceID = "https://www.hms-networks.com/industrial-iot-solutions";

int get_mac_test(const char *file)
{
#define MAX_TRIAL 500
  char macs[MAX_TRIAL][18];
  char *hashes[MAX_TRIAL];
  FILE *pFile = fopen(file, "r");
  int count ,i , j = 0 ,  err = 0;
  char* hash;
  if (pFile != NULL)
  {
    while (!feof(pFile))
    {
      count = fscanf(pFile, "%s\n", macs[j++]);
      //puts(macs[j-1]);
    }
    fclose(pFile);

    for (i = 0; i < j; i++)
    {
      hashes[i] = gen_hash_string(macs[i]);
      //printf("%s\n", hashes[i]);
    }
     
    {
      int k = 0;
      for( i =0; i < j; i++){
        for(k = 0; k < j ; k++ ){
          if( k != i ){
            if(0 == strncmp(hashes[i], hashes[k], 16 ) ){
              err = 1;
              printf("%d %s\n",i+1,macs[i]);
            }
          }
        }
      }
    }
  
    for (i = 0; i < j; i++)
    {
      free(hashes[i]);
    }
    if(err) return 1 ;    
  }
  else
  {
    perror("file open error\n");
    return 1;
  }
  return 0;
}

int main(void)
{
  get_mac_test("./mac_file.txt");
  return 0;
}