#include "helper.h"
#include <dirent.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>

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
            }
            closedir(dir);
            strcat(json_request,"]}");
            printf("send:%s\n",json_request);
            send_length(client_socket, strlen(json_request));
            sendall(client_socket, json_request);
            memset(json_request, 0, sizeof(json_request));
    }
    else
    {
            perror("opendir error:");
    }
}

void send_file(int client_socket)
{
        char * file_name = recv_file_name(client_socket, file_name);
        char file_to_send[100] = "files_on_server/";
        strcat(file_to_send,file_name);
        printf("file_to_send:%s\n",file_to_send);

        FILE *fp;
        fp = fopen(file_to_send,"r");
        if (fp == NULL)
        {
                printf("File not created okay, errno = %d\n", errno);
        }

        fseek(fp, 0L,SEEK_END);
        int length = ftell(fp);
        send_length(client_socket,length);
        fclose(fp);
        fp = fopen(file_to_send,"r");

        int max = 100;
        char buff[max];
        memset(buff, 0, sizeof(buff));
        while(fgets(buff, max, fp) != NULL)
        {
                sendall(client_socket, buff);
                length -= strlen(buff);
                if (length < 100)
                {
                    max = length;
                }
                memset(buff, 0, sizeof(buff));

        }
        fclose(fp);
        memset(file_to_send, 0, sizeof(file_to_send));

}

void recv_file(int client_socket)
{
        char * file_name = recv_file_name(client_socket, file_name);
        char file_to_recv[100] = "files_on_server/";
        strcat(file_to_recv,file_name);
        printf("file_to_recv:%s\n",file_to_recv);

        FILE *fp;
        fp = fopen(file_to_recv,"w");
        if (fp == NULL)
        {
                printf("File not created okay, errno = %d\n", errno);
        }
        char buff[MAXDATASIZE];
        memset(buff, 0, sizeof(buff));
        int length = recv_length(client_socket);
        int max = 100;
        if (length < 100)
        {
            max = length;
        }
        int count;
        while((count = recv(client_socket,buff, max, 0)) > 0)
        {
                fprintf(fp,"%s",buff);
                printf("\ncount:%ld 1.%s",count,buff);
                memset(buff, 0, sizeof(buff));
                length -= count;
                if (length <= 100)
                {
                    max = length;
                }
                if (max == 0)
                {
                    break;
                }
        }
        fclose(fp);
        memset(file_to_recv, 0, sizeof(file_to_recv));
}

void remove_file(int client_socket)
{
        char * file_name = recv_file_name(client_socket, file_name);
        char file_to_remove[100] = "files_on_server/";
        strcat(file_to_remove,file_name);
        printf("file_to_remove:%s\n",file_to_remove);

        int removed = remove(file_to_remove);
        if (removed == 0)
        {
            printf("File:%s deleted successfully\n", file_to_remove);
        }
        else
        {
            printf("Error: unable to delete the file:%s\n", file_to_remove);
        }
        memset(file_to_remove, 0, sizeof(file_to_remove));
}
