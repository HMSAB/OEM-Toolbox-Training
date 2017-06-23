#include <errno.h>
#include <openssl/md5.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <uuid/uuid.h>

static const char *spaceID = "https://www.hms-networks.com/industrial-iot-solutions";

static char *add_hyphen(char *uuid)
{
  char *uuidHyphen = malloc(44);
  char worker[16];

  strncpy(uuidHyphen, uuid, 8);
  uuidHyphen[8] = '\0';
  strcat(uuidHyphen, "-");

  strncpy(worker, &uuid[8], 4);
  worker[4] = '\0';
  strcat(uuidHyphen, worker);
  strcat(uuidHyphen, "-");

  strncpy(worker, &uuid[12], 4);
  worker[4] = '\0';

  strcat(uuidHyphen, worker);
  strcat(uuidHyphen, "-");

  strncpy(worker, &uuid[16], 4);
  worker[4] = '\0';
  strcat(uuidHyphen, worker);
  strcat(uuidHyphen, "-");

  strncpy(worker, &uuid[20], 14);
  strcat(uuidHyphen, worker);

  free(uuid);
  return uuidHyphen;
}

/*
  returns char* that must be freed
*/
char *gen_hash_string(char *str)
{
  MD5_CTX context;
  unsigned char digest[16];

  /*output will be freed in add_hyphen*/
  char *output = (char *)malloc(33);

  MD5_Init(&context);
  MD5_Update(&context, spaceID, strlen(spaceID));
  MD5_Update(&context, str, strlen(str));
  MD5_Final(digest, &context);

  digest[4] &= 0x0F;
  digest[4] |= 0x30; // type 3
  digest[8] &= 0x3F;
  digest[8] |= 0xA0; // variant 1

  {
    int i = 0;
    for (; i < 16; i++)
    {
      snprintf(&(output[i * 2]), 16 * 2, "%02X", (unsigned int)digest[i]);
    }
  }
  return add_hyphen(output);
}

int get_mac_addr(char *mac, const char *file)
{
  FILE *pFile = fopen(file, "r");
  if (pFile != NULL)
  {
    int count = fscanf(pFile, "%s\n", mac);
    fclose(pFile);
    //printf("size %d MAC addr: %s\n", count, mac);
  }
  else
  {
    perror("file open error\n");
    return -1;
  }
  return 0;
}