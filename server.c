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

int sendall(int client_socket, char *buf);
//int checklog(int client_socket, char *login, char *haslo);

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
	memset(buf,'\0', sizeof(buf));
	memset(login,'\0', sizeof(login));
	memset(haslo,'\0', sizeof(haslo));
	
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

	// TO DO: instead of this, implement checking user from file
int i = checklog(client_socket, login,  haslo); // char *login, char *haslo));

		if( i == -1)
		{
			if (sendall(client_socket, "TAK\n") == -1)
			{
			
				perror("sendall error");
			}
		}
		else
		{
			if (sendall(client_socket, "NIE\n") == -1)
			{
		
				perror("sendall error");
			}
		}


	close(client_socket);
	close(server_socket);

	return 0;
}



int checklog(int client_socket, char *login, char *haslo)// char *login, char *haslo)
{
typedef enum  {true = 1, false = 0} bool;
char const* const logins;
FILE *file = fopen("logins", "r");
char line[512];
char *search = " "; 
char *log;
bool find = false;
 while (fgets(line, sizeof(line), file))
 {
        log = strtok(line, search);
        while( log != NULL )
        {
                if((strcmp(log, login)) == 0)
                {
                        printf("\n%s", log);
                        log = strtok(NULL, search);
                        continue;
                }
                else if((strcmp(log, haslo)) == 0)
                {
                        printf("\n%s", log);
                        find = true;
                        break;
                }
                else
                {
                break;
                }
        }
	if(find == true)
        {
                printf("\nFound\n");
                break;
        }
        else
        {
                printf("\nNot found\n");
        }
 }
        fclose(file);
 	if(find == true) 
	{
		return (-1);
	}
	else
	{
		return (0);
	}

  	 
		//fclose(file);
}

int sendall(int client_socket, char *buf)
{
	int total_sent = 0;
	int bytes_left = sizeof(buf);
	int sent_bytes;

	while(total_sent < sizeof(buf))
	{
		sent_bytes = send(client_socket, buf+total_sent, bytes_left, 0);
		if (sent_bytes == -1)
		{
			printf("Sent only %d of %lu bytes\n", total_sent, sizeof(buf));
			break;
		}
		total_sent += sent_bytes;
		bytes_left -= sent_bytes;
	}

	return (sent_bytes == -1) ? -1 : 0;
}

