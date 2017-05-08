#ifndef MESSAGE_H
#define MESSAGE_H

/**********************************************502
*
*****************************************/
#define DEV_TEST
#define GUID_SIZE 37
#define MBTCP_PORT 502

#define GEN_ENUM(ENUM) ENUM,
#define GEN_STRING(STRING) #STRING,

/***********************************
*  Function Types - Holding, Input
*    usage example: FUNCTION[Holding]
*
************************************/
#define FOREACH_FUNCTION(FUNCT)                                                \
  FUNCT(Holding)                                                               \
  FUNCT(Input)

enum FUNCTION { FOREACH_FUNCTION(GEN_ENUM) };

static const char *FUNCTION[] = {FOREACH_FUNCTION(GEN_STRING)};

/***********************************
*  Command Types - read, write
*    usage example: COMMAND[read]
*
************************************/
#define FOREACH_COMMAND(COMMAND)                                               \
  COMMAND(read)                                                                \
  COMMAND(write)

enum COMMAND { FOREACH_COMMAND(GEN_ENUM) };

static const char *COMMAND[] = {FOREACH_COMMAND(GEN_STRING)};

/***********************************
*  Value Types - S8, U8, ......
*    usage example: VALUE_TYPE[S8]
*
************************************/
#define FOREACH_VALUE_TYPE(VALUE_TYPE)                                         \
  VALUE_TYPE(S8)                                                               \
  VALUE_TYPE(U8)                                                               \
  VALUE_TYPE(S16)                                                              \
  VALUE_TYPE(U16)                                                              \
  VALUE_TYPE(S32)                                                              \
  VALUE_TYPE(U32)                                                              \
  VALUE_TYPE(S64)                                                              \
  VALUE_TYPE(U64)

enum VALUE_TYPE { FOREACH_VALUE_TYPE(GEN_ENUM) };

static const char *VALUE_TYPE[] = {FOREACH_VALUE_TYPE(GEN_STRING)};

#endif