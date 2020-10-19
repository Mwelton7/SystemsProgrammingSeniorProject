#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

void create_f(char * filepath);
void create_d(char * dirpath);
void create_h(char * oldname,char * linkname);
void create_s(char * oldname,char * linkname);

int main(int argc, char *argv[]){

if(argv[1]!=NULL){
if(strcmp(argv[1], "-f") == 0)
{
    create_f(argv[2]);
}
if(strcmp(argv[1], "-d") == 0)
{
    create_d(argv[2]);
}
if(strcmp(argv[1], "-h") == 0)
{
    create_h(argv[2],argv[3]);
}
if(strcmp(argv[1], "-s") == 0)
{
    create_s(argv[2],argv[3]);
}

}
else {
printf("Incorrect argument./n");
}


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
