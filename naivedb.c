#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "interfaceFunctions.c"
#include <stdbool.h>
#define MAX_CMD_SIZE 256

int main(int argc, char *argv[]){
	char *command = malloc(MAX_CMD_SIZE);
	char *scriptname = malloc(MAX_CMD_SIZE);
	FILE *script;
	//char *commandArray; 
    //commandArray = (char *)malloc(10 *sizeof(char));
	char *commandArray[10]; //array to store tokenized command in (ie, individual commands split by whitespace)
	bool isRedirect;
	int redirIndex = 0;
	if (argc>1) {
		strcpy(scriptname,argv[1]);
		script = fopen(scriptname,"r");
	}
	for(int j=0; j>=0; j++){ 
	//the infinite loop in which our command line operates until quit command is given
		if(argc>1){
			if (fgets(command, MAX_CMD_SIZE,script)!=NULL){
				printf(command);
				} //if the user enters a script file, read it line by line until eof, then exit
			else exit(0);
		}
		else {printf("Enter a command>");
			fgets(command, MAX_CMD_SIZE, stdin); //else let the user enter commands
		}
		int i=0;
		commandArray[i] = strtok(command," ");
		while(commandArray[i]!=NULL){
			commandArray[++i] = strtok(NULL," "); //split up the whole line of input into individual args
			}
		
		/* for (int f=0; f<10; f++){
			if (strcmp(commandArray[f],">")==0){
				isRedirect=true; //check if any command requires redirect of output
				redirIndex = f;
				break; //this block causes a segfault and we can't figure out why
			}
		} */
		 
	
		if (strncmp(command, "create", 6)==0){
			pid_t cpid;
			if ((cpid = fork())==0) {
				execv("create",commandArray);	//fork, child process executes create.c and sends the command array as args	
				exit(0);     //end child proc once it's done
			}					
			else {
				cpid = wait(NULL); //blocking til child finishes
			} 
		}
		else if (strncmp(command, "quit", 4)==0) {
			
			if (quit() == 0) {
				if (argc>1) {fclose(script);} //quits using the function from interfaceFunctions
				exit(0);
			}
		}
		else if (strcmp(commandArray[0],"fileconverter")==0){
			pid_t child;
			if ((child = fork()) == 0) {
				execv("fileconverter",commandArray); //fork, child process executes create.c and sends the command array as args	
				exit(0);     //end child proc once it's done
			}					
			else {
				child = wait(NULL); //blocking til child finishes
			}
		}
		else if (strcmp(commandArray[0],"indexer")==0){
			pid_t child2;
			if ((child2 = fork())==0) {
				execv("indexer",commandArray);	//fork, child process executes create.c and sends the command array as args	
				exit(0);     //end child proc once it's done
			}				
			else {
				child2 = wait(NULL); //blocking til child finishes
			}
			//srchindx
		} 
		else if (strcmp(commandArray[0],"srchindx")==0){
			pid_t child3;
			if ((child3 = fork())==0) {
				execv("srchindx",commandArray);	//fork, child process executes create.c and sends the command array as args	
				exit(0);     //end child proc once it's done
			}				
			else {
				child3 = wait(NULL); //blocking til child finishes
			}
			//srchindx
		} 
		else {
			printf("Unrecognized command.\n");
		} 			
			
			
			/* if (isRedirect) {
			pid_t cpid; 
			if (fork()== 0) {{execv("create",commandArray);}	//fork, child process executes create.c and sends the command array as args	
				exit(0);     //end child proc once it's done
			}					
			else {
				cpid = wait(NULL); //blocking til child finishes
			}  */
	}
}
