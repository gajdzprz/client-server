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

	printf("Podaj login:");
	scanf("%s",buf);
	send(client_socket, buf, strlen(buf), 0);
	memset(buf,'\0', sizeof(buf));

	printf("Podaj haslo:");
	scanf("%s",buf);
	send(client_socket, buf, strlen(buf), 0);
	memset(buf,'\0', sizeof(buf));

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

	close(client_socket);

	return 0 ;
}
