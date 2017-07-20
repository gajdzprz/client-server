#ifndef MENU_H
#define MENU_H

void send_files_list(char * json_request, int client_socket);

void send_file(int client_socket);

void recv_file(int client_socket);

void remove_file(int client_socket);

#endif
