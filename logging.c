#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "jsmn/jsmn.h"

int checklog(jsmntok_t * ptr_tokens,char * json_response)
{
        char login[100], haslo[100];
        memset(login, 0, sizeof(login));
        memset(haslo, 0, sizeof(haslo));
        strncat(login,json_response + ptr_tokens[2].start, ptr_tokens[2].end - ptr_tokens[2].start);
        strncat(haslo,json_response + ptr_tokens[4].start, ptr_tokens[4].end - ptr_tokens[4].start);

        char const* const logins = "logins";
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
