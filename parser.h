#ifndef PARSER_H
#define PARSER_H

#include "jsmn/jsmn.h"

jsmntok_t * parser_jsmn(char * json_response,jsmntok_t * tokens, int size);

#endif
