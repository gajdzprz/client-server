#include "helper.h"
#include "parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>

// jsmn include
#include "jsmn/jsmn.h"

int sendall(int client_socket, char * json_request)
{
        int total_sent = 0;
        int bytes_left = strlen(json_request);
        int sent_bytes;

        while(total_sent < strlen(json_request))
        {
                sent_bytes = send(client_socket, json_request+total_sent, bytes_left, 0);
                if (sent_bytes == -1)
                {
                        printf("Sent only %d of %lu bytes\n", total_sent, sizeof(json_request));
                        break;
                }
                total_sent += sent_bytes;
                bytes_left -= sent_bytes;
        }

        return (sent_bytes == -1) ? -1 : 0;
}

void recv_response(int client_socket, char * json_response)
{
    memset(json_response, 0, sizeof(json_response));
    int numbytes;
    numbytes = recv(client_socket, json_response, MAXDATASIZE-1, 0);
    if (numbytes == -1)
    {
            perror("json_response error");
            exit(1);
    }
    else if (numbytes == 0)
    {
            printf("Lost connection, client disconnected\n");
    }
    else
    {
            printf("recv:%s\n",json_response); // later can be removed
    }
}

int recv_choice(int client_socket)
{
    char json_response[MAXDATASIZE];
    memset(json_response, 0, sizeof(json_response));
    recv_response(client_socket, json_response);

    jsmntok_t * ptr_tokens;
    ptr_tokens = parser_jsmn(json_response, ptr_tokens, 3);

    char choice_char[3];
    memset(choice_char, 0, sizeof(choice_char));
    strncat(choice_char,json_response + ptr_tokens[2].start, ptr_tokens[2].end - ptr_tokens[2].start);
    int client_choice = atoi(choice_char);
    memset(json_response, 0, sizeof(json_response));
    free(ptr_tokens);
    return client_choice;
}

char * recv_file(int client_socket, char * file_name)
{
    static char json_response[MAXDATASIZE];
    memset(json_response, 0, sizeof(json_response));
    recv_response(client_socket, json_response);

    jsmntok_t * ptr_tokens;
    ptr_tokens = parser_jsmn(json_response, ptr_tokens, 3);

    file_name = (char *) malloc(MAXDATASIZE * sizeof(*file_name));
    memset(file_name, 0, sizeof(file_name));
    strncat(file_name,json_response + ptr_tokens[2].start, ptr_tokens[2].end - ptr_tokens[2].start);
    free(ptr_tokens);
    return file_name;
}
