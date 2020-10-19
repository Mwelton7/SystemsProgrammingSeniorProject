#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>
//#include <sys/types.h>
//#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#define MAX_LINE BUFSIZ



void create_f(char * filepath);
void create_d(char * dirpath);
void create_h(char * oldname,char * linkname);
void create_s(char * oldname,char * linkname);


int quit() {
		char line[MAX_LINE];
		printf(
        "Are you sure you want to exit? All files will be lost! "
        "Y/n ");
			fgets(line, MAX_LINE, stdin);
			if (strncmp(line, "y", 1) == 0 || strncmp(line, "Y", 1) == 0) {
				return 0;
			}
		else {return 1;}
	}
	
int create(char* command) {

if (strlen(command)>8){
	if(strncmp(command, "create -f", 8) == 0)
	{
		printf(&command[8]);
		//create_f(&command[8])
	}
	else if(strncmp(command, "create -d", 8) == 0)
	{
		//create_d(argv[2]);
	}
	else if(strncmp(command, "create -h", 8) == 0)
	{
		//create_h(argv[2],argv[3]);
	}
	else if(strncmp(command, "create -s", 8) == 0)
	{
		//create_s(argv[2],argv[3]);
	}

	}
	else {
		printf("Incorrect argument for: -\n");
	}


/* if(argv[1] == NULL)
{
	printf("Incorrect number of arguments.\n");

} */


return 0;
}


void create_f(char * filepath)
{
    if(creat(filepath,0640)<0)
        printf("create file %s failed\n",filepath);
}

void create_d(char * dirpath)
{
    if(mkdir(dirpath,0750)<0)
        printf("create directory %s failed\n",dirpath);
}

void create_h(char * oldname,char * linkname)
{
    if(link(oldname,linkname)<0)
        printf("create hard link %s failed\n",linkname);
}

void create_s(char * oldname,char * linkname)
{
    if(symlink(oldname,linkname)<0)
        printf("create symbol link %s failed\n",linkname);
}

	