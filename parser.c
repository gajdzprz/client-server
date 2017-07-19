#include "parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

jsmntok_t * parser_jsmn(char * json_response, jsmntok_t * tokens, int size)
{
    jsmn_parser parser;
    jsmn_init(&parser);
    tokens =(jsmntok_t *) malloc(size * sizeof(*tokens));
    printf("chce sparsowac:%s\n",json_response); // to delete
    int num_tokens = jsmn_parse(&parser, json_response, strlen(json_response), tokens, size);
    if ( num_tokens < 0)
    {
            printf("jsmn_parse error\n");
            exit(1);
    }
    memset(json_response, 0, sizeof(json_response));
    return tokens;
}
