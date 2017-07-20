#ifndef HELPER_H
#define HELPER_H

#define MAXDATASIZE 100

int sendall(int client_socket, char * json_request);

void recv_response(int client_socket, char * json_response);

int recv_choice(int client_socket);

char * recv_file_name(int client_socket, char * file_name);

#endif
