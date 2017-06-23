#ifndef UUID_H
#define UUID_H

char *add_hyphen(char *uuid);
char *gen_hash_string(char *str);
int get_mac_addr(char *mac, const char *file);

#endif