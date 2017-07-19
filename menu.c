#include "menu.h"
#include "helper.h"
#include <dirent.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void send_files_list(char * json_request, int client_socket)
{
    DIR *dir;
    struct dirent *read_dir;
    dir = opendir("files_on_server/");
    if (dir)
    {
            strcat(json_request,"{\"files\":[ ");
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
            memset(json_request, 0, sizeof(json_request));
    }
    else
    {
            perror("opendir error:");
    }
}


// refactor! probably is not needed to searching first file
void send_file(int client_socket)
{
    char * file_name = recv_file(client_socket, file_name);
    char file_to_send[100] = "files_on_server/";
    strcat(file_to_send,file_name);
    printf("plik:%s\n",file_to_send);

    // send whole file
        FILE *fp;
        fp = fopen(file_to_send,"r");
        if (fp == NULL)
        {
                printf("File not created okay, errno = %d\n", errno);
        }
        char buff[MAXDATASIZE];
        while(fgets(buff, MAXDATASIZE, fp) != NULL)
        {
                sendall(client_socket, buff);
                memset(buff, 0, sizeof(buff));
        }
        fclose(fp);
        memset(file_to_send, 0, sizeof(file_to_send));

}
