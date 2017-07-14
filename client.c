#include <errno.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>

#define PORT 8081
#define MAXDATASIZE 100

void display_menu(void);
void display_files(void);

int main(int argc, char *argv[])
{
	int client_socket, numbytes;
	char buf[MAXDATASIZE];
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

	// creating json login
	char js[100] = "{\"login\": \"";

	printf("Podaj login:");
	scanf("%s",buf);

	strcat(js,buf);

	memset(buf,'\0', sizeof(buf));

	printf("Podaj haslo:");
	scanf("%s",buf);

	strcat(js,"\", \"password\": \"");
	strcat(js,buf);

	strcat(js,"\"}");

	printf("json:%s\n",js);

	send(client_socket, js, strlen(js), 0);
	memset(buf,'\0', sizeof(buf));
	memset(js,'\0', sizeof(buf));

	numbytes = recv(client_socket,buf,MAXDATASIZE-1,0);
	if (numbytes == -1)
	{
		perror("Sign in error");
		exit(1);
	}
	else if (numbytes == 0)
	{
		printf("Lost connection, server disconnected\n");
	}
	else
	{
		printf("%s",buf);
	}

	int choice;
	display_menu();
	scanf("%d", &choice);

	if (choice > 0 && choice < 4)
	{
		display_files();
		// TO DO: next if statement with choice of which file 
		// want to download or remove
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

void display_menu(void)
{
	printf("-------MENU-------\n");
	printf("1 - display files\n");
	printf("2 - download file\n");
	printf("3 - remove file\n");
	printf("4 - send file\n");
}

// TO DO: download from server list of files
void display_files(void)
{
	printf("-------MENU-------\n");
	printf("abecadlo.txt\n");
	printf("foto.png\n");
	printf("index.html\n");
	printf("etc...\n");
}
