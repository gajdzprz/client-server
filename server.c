#include <arpa/inet.h>
#include <dirent.h>
#include <errno.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

// jsmn include
#include "jsmn/jsmn.h"

// internal include
#include "helper.h"
#include "logging.h"
#include "menu.h"
#include "parser.h"

#define PORT 8081
#define MAXUSERS 5
#define MAXDATASIZE 100

int main(int argc, char *argv[])
{
	int server_socket, client_socket;
	struct sockaddr_in server_addr, client_addr;
        char json_request[MAXDATASIZE], json_response[MAXDATASIZE];
        char * js;
	int size;
	int yes = 1;

	server_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (server_socket == -1)
	{
		perror("socket error");
		exit(1);
	}

	if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1)
	{
		perror("setsockopt error");
		exit(1);
	}

	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(PORT);
	server_addr.sin_addr.s_addr = inet_addr("10.0.2.15");
        memset(&(server_addr.sin_zero), 0, 8);

	if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(struct sockaddr)) == -1)
	{
		perror("bind error");
		exit(1);
	}

	if (listen(server_socket, MAXUSERS) == -1)
	{
		perror("listen error");
		exit(1);
	}
	size = sizeof(struct sockaddr_in);
	client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &size);
	if (client_socket == -1)
	{
		perror("accept error");
		exit(1);
	}
        printf("server: got connection from %s\n", inet_ntoa(client_addr.sin_addr));


        // only for test
        int length = recv_length(client_socket);
        js = (char *) malloc(length * sizeof(char *));

        // recv json with login and password
        memset(js, 0, sizeof(js));
        recv_response(client_socket, js, length);

        // parse json with login and password into tokens
        jsmntok_t * ptr_tokens;
        ptr_tokens = parser_jsmn(js, ptr_tokens, 5);

        memset(json_request, 0, sizeof(json_request));
        // checking login and password
        int log = checklog(ptr_tokens,js);
	if( log == 0)
	{
                strcat(json_request, "{\"checklog\": \"YES\"}");
                send_length(client_socket, strlen(json_request)+1);
		if (sendall(client_socket, json_request) == -1)
		{
                        perror("sendall error");
                }
	}
	else
	{
                strcat(json_request, "{\"checklog\": \"NO\"}");
                send_length(client_socket, strlen(json_request)+1);
		if (sendall(client_socket, json_request) == -1)
		{
                        perror("sendall error");
		}
                printf("send:%s\n", json_request);
                free(ptr_tokens);
                exit(1);
	}
	printf("send:%s\n", json_request);
        free(ptr_tokens);

        memset(json_response, 0, sizeof(json_response));
        memset(json_request, 0, sizeof(json_request));

        length = recv_length(client_socket);
        // waiting for choice
        int choice = recv_choice(client_socket, length);
        if (choice > 0 && choice < 4)
        {
            // sending list of files
            send_files_list(json_request, client_socket);

            if (choice == 2)
            {
                // sending whole file
                send_file(client_socket);
            }
            else if (choice == 3)
            {
                // remove file
                remove_file(client_socket);
            }
        }
        else if (choice == 4)
        {
            // download file from client
            recv_file(client_socket);
        }
        else
        {
            printf("Wrong number\n");
        }

        free(js);
	close(client_socket);
	close(server_socket);
	return 0;
}
