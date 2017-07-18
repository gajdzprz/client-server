#include <errno.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>

// jsmn include
#include "../jsmn/jsmn.h"

#define PORT 8081
#define MAXDATASIZE 100

void send_choice(int client_socket, int choice);
void logging(char * json_request, char * json_response, int client_socket);
void display_menu(void);
void display_files(char * json);

int main(int argc, char *argv[])
{
	int client_socket, numbytes;
	char json_request[MAXDATASIZE], json_response[MAXDATASIZE];
	struct hostent *host;
	struct sockaddr_in their_addr;

	if (argc != 2)
	{
		fprintf(stderr,"usage: client hostname\n");
		exit(1);
	}

	if ((host = gethostbyname(argv[1])) == NULL) 
	{
		perror("gethostbyname");
		exit(1);
	}

	if ((client_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		perror("socket");
		exit(1);
	}

	their_addr.sin_family = AF_INET;
	their_addr.sin_port = htons(PORT);
	their_addr.sin_addr = *((struct in_addr *)host->h_addr);
	memset(&(their_addr.sin_zero), '\0', 8);
	
	if (connect(client_socket, (struct sockaddr *)&their_addr, sizeof(struct sockaddr)) == -1) 
	{
		perror("connect");
		exit(1);
	}

	logging(json_request, json_response, client_socket);

	memset(json_request, '\0', sizeof(json_request));
	int choice;
	display_menu();
        scanf("%d", &choice);

        // display files
	if (choice > 0 && choice < 4)
        {
                send_choice(client_socket,choice);
		memset(json_response, '\0', sizeof(json_response));
		int numb = recv(client_socket, json_response, 99, 0);
		display_files(json_response);
		// TO DO: next if statement with choice of which file 
		// want to download or remove

                // download file
		if (choice == 2)
		{
                        scanf("%d", &choice);
			memset(json_request,'\0',sizeof(json_request));
			memset(json_response, '\0', sizeof(json_response));
                        send_choice(client_socket,choice);

                        // recv file
			FILE *fp;
			fp = fopen("files_download/humans.txt","w");
			if (fp == NULL)
			{
				printf("File not created okay, errno = %d\n", errno);
				exit(1);
			}
			while(recv(client_socket,json_response, 99, 0) > 0)
			{
				fprintf(fp,"%s",json_response);
				memset(json_response, '\0', sizeof(json_response));
			}
			fclose(fp);
		}
                // remove file
                else if (choice == 3)
                {
                        scanf("%d", &choice);
                        send_choice(client_socket,choice);
                }
	}
	else if (choice == 4)
	{
		printf("Write path to the file:\n");
	}
	else
	{
		printf("Wrong number!\n");
	}

	close(client_socket);

	return 0 ;
}

void send_choice(int client_socket, int choice)
{
    char json_request[MAXDATASIZE];
    char choice_char[3];
    int num_send;
    strcat(json_request, "{\"choice\": \"");
    sprintf(choice_char, "%d", choice);
    strcat(json_request, choice_char);
    strcat(json_request, "\"}");
    num_send = send(client_socket, json_request, strlen(json_request), 0);
    if (num_send == -1)
    {
        perror("send error");
    }
    printf("send:%s\n", json_request);
    memset(json_request,'\0',sizeof(json_request));
}

void logging(char * json_request, char * json_response, int client_socket)
{
	char buf[MAXDATASIZE];
	// creating json login
	memset(json_request, '\0', sizeof(json_request));
	strcat(json_request, "{\"login\": \"");

	printf("Podaj login:");
	scanf("%s",buf);

	strcat(json_request,buf);
	memset(buf,'\0', sizeof(buf));

	printf("Podaj haslo:");
	scanf("%s",buf);

	strcat(json_request,"\", \"password\": \"");
	strcat(json_request,buf);
	strcat(json_request,"\"}");

	printf("send:%s\n",json_request);

	send(client_socket, json_request, strlen(json_request), 0);
	memset(buf,'\0', sizeof(buf));
	memset(json_request,'\0', sizeof(json_request));

	int numbytes = recv(client_socket,json_response,MAXDATASIZE-1,0);
	if (numbytes == -1)
	{
		perror("Sign in error");
		exit(1);
	}
	else if (numbytes == 0)
	{
		printf("Lost connection, server disconnected\n");
		exit(1);
	}
	else
	{
		printf("recv:%s\n",json_response);
		jsmn_parser parser;
		jsmn_init(&parser);
		jsmntok_t tokens[5];
		if (jsmn_parse(&parser, json_response, strlen(json_response), tokens, 10) < 0)
		{
			printf("jsmn_parse error\n");
			exit(1);
		}
		else
		{
			strncat(buf, json_response + tokens[2].start, tokens[2].end - tokens[2].start);
			if ((strcmp(buf, "tak")) == 0)
			{
				printf("log on\n");
			}
			else
			{
				printf("not log on\n");
				exit(1);
			}
			memset(buf, '\0', sizeof(buf));
		}
	}
	memset(json_response, '\0', sizeof(json_request));
}

void display_menu(void)
{
	printf("-------MENU-------\n");
	printf("1 - display files\n");
	printf("2 - download file\n");
	printf("3 - remove file\n");
	printf("4 - send file\n");
}

void display_files(char * json_response)
{
	printf("recv:%s\n",json_response);
	jsmn_parser parser;
	jsmn_init(&parser);
	jsmntok_t tokens[100];
	int num_files = jsmn_parse(&parser, json_response, strlen(json_response), tokens, 100);
	int i;
	printf("-------MENU-------\n");
	for (i = 3; i<num_files; i++)
	{
		if (tokens[i].type == JSMN_STRING)
		{
			printf("%d. %.*s\n",i-2,tokens[i].end - tokens[i].start, json_response + tokens[i].start);
		}
	}
}
