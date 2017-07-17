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
#include "../jsmn/jsmn.h"

#define PORT 8081
#define MAXUSERS 5
#define MAXDATASIZE 100

int checklog(char *login, char *haslo);
int sendall(int client_socket, char *json_request);
int main(int argc, char *argv[])
{
	int server_socket, client_socket;
	struct sockaddr_in server_addr, client_addr;
	char json_request[MAXDATASIZE], json_response[MAXDATASIZE];
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
	memset(&(server_addr.sin_zero), '\0', 8);

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
	char login[100], haslo[100];

	memset(json_response,'\0',sizeof(json_response));
	memset(login,'\0', sizeof(login));
	memset(haslo,'\0', sizeof(haslo));
  	int numbytes;
	numbytes = recv(client_socket, json_response, 99, 0);
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
	jsmn_parser parser;
	jsmn_init(&parser);
	jsmntok_t tokens[5];
	if (jsmn_parse(&parser, json_response, strlen(json_response), tokens, 10) < 0)
	{
		printf("jsmn_parse error\n");
		exit(1);
	}

	strncat(login,json_response + tokens[2].start, tokens[2].end - tokens[2].start);
	strncat(haslo,json_response + tokens[4].start, tokens[4].end - tokens[4].start);
	memset(json_request, '\0', sizeof(json_request));
	strcat(json_request, "{\"checklog\": ");
	int log = checklog(login,  haslo);
	if( log == 0)
	{
		strcat(json_request, "\"tak\"}");
		if (sendall(client_socket, json_request) == -1)
		{
			perror("sendall error");
		}
	}
	else
	{
		strcat(json_request, "\"nie\"}");
		if (sendall(client_socket, json_request) == -1)
		{
			perror("sendall error");
		}
	}
	printf("send:%s\n", json_request);

	memset(json_response,'\0',sizeof(json_response));
	numbytes = recv(client_socket, json_response, 99, 0);
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

	memset(json_response,'\0',sizeof(json_response));
	memset(json_request, '\0', sizeof(json_request));
	strcat(json_request,"{\"files\":[ ");
	DIR *dir;
	struct dirent *read_dir;
	dir = opendir("files_on_server/");
	if (dir)
	{
		while ((read_dir = readdir(dir)) != NULL)
		{
			// checking whether it is normal file
			if (read_dir->d_type != DT_REG)
			{
				continue;
			}
			strcat(json_request,"\"");
			strcat(json_request,read_dir->d_name);
			strcat(json_request,"\", ");
			printf("%s\n", read_dir->d_name);
		}
		closedir(dir);
		strcat(json_request,"]}");
		printf("send:%s\n",json_request);
		sendall(client_socket, json_request);
		memset(json_request,'\0',sizeof(json_request));
	}
	else
	{
		perror("opendir error:");
	}

	close(client_socket);
	close(server_socket);
	return 0;
}

int checklog(char *login, char *haslo)
{
	char const* const logins = "logins"; //not needed? or use instead of "logins"?
	FILE *file = fopen(logins, "r");
	char line[512];
	char *search = " ";
	char *log;
	while (fgets(line, sizeof(line), file))
	{
		log = strtok(line, search);
		while( log != NULL )
		{
			if((strcmp(log, login)) == 0)
			{
				printf("login found:%s\n", log);
				log = strtok(NULL, search);
				continue;
			}
			else if((strcmp(log, haslo)) == 0)
			{
				printf("password found:%s\n", log);
				printf("Found\n");
				return 0;
			}
			else
			{
				break;
			}
		}
	}
        fclose(file);
	printf("Not found\n");
	return -1;
}

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
