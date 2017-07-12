#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define PORT 8081
#define MAXUSERS 5

int main(int argc, char *argv[])
{
	int server_socket, client_socket;
	struct sockaddr_in server_addr, client_addr;
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

	int numbytes;
	char buf[100], login[100], haslo[100];

	numbytes = recv(client_socket, login, 99, 0);
	if (numbytes == -1)
	{
		perror("Login error");
		exit(1);
	}
	else if (numbytes == 0)
	{
		printf("Lost connection, client disconnected\n");
	}
	else
	{
		printf("%s\n",login); // later can be removed
	}

	numbytes = recv(client_socket, haslo, 99, 0);
	if (numbytes == -1)
	{
		perror("Password error");
		exit(1);
	}
	else if (numbytes == 0)
	{
		printf("Lost connection, client disconnected\n");
	}
	else
	{
		printf("%s\n",haslo); // later can be removed
	}

	// TO DO: insteda of this, implement checking user from file
	if ((strcmp(login, "maslo") && strcmp(haslo, "qwerty")) == 0)
	{
		send(client_socket, "TAK\n", strlen("TAK\n"), 0);
	}
	else
	{
		send(client_socket, "NIE\n", strlen("NIE\n"), 0);
	}

	close(client_socket);
	close(server_socket);

	return 0;
}
