#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[])
{
typedef enum  {true = 1, false = 0} bool;
char const* const logins = argv[1];
FILE *file = fopen("logins", "r");
char line[512];
char *search = " "; 
char *log;
char *str = "qwe";
char *str1 = "rty";
bool find = false;
 while (fgets(line, sizeof(line), file))
 {
	log = strtok(line, search);
	while( log != NULL )
	{
		if((strcmp(log, str)) == 0)
		{
			printf("\n%s", log);
			log = strtok(NULL, search);
			continue;
		}
	        else if((strcmp(log, str1)) == 0)
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
		printf("\nZnalezione\n");
		break;
	}
	else
	{
		printf("\nNieznaleziono\n");
	}

 }
	fclose(file);
	return 0;
}
