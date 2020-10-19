#define MAX 100
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>



//*****READ ME IF INTERFACE DOES NOT WORK******
//type: gcc indexer.c
//./a.out flightdata.bin outdirectory
void create_f(char * filepath);
void create_d(char * dirpath);
char * makeFileName(char * aircode, char * outdirectory);
int compareDates(const void *a,const void *b);


    typedef struct Airline{
        char code[8];
        char origin[4];
        char destination[4];
        char date[11];
        char time[6];
        
    }Airline[MAX];
    
    int numFlights = 0;
    char file_path[100];
   char outdirectory[100];
   

int main(int argc, char* argv[]){
		if(argc < 3){
		printf("Error wrong number of arguments. Not enough.\n");
		exit(1);
	}
		if(argc > 3){
		printf("Error wrong number of arguments. Too many.\n");
		exit(1);
	}
	
    //dynamically allocate memory for up to a maximum of 100 Airline structs
    Airline* air = (Airline*)malloc(MAX * sizeof(Airline));
    FILE *bin, *out;
    

   strcpy(outdirectory,argv[2]);
        
    //prompt user to enter desired directory to store files
    if(access(outdirectory,F_OK)==-1){
        create_d(outdirectory);
    }
    //open flightdata.bin for reading binary store in FILE * bin
    if((bin = fopen(argv[1],"rb"))== NULL){
        printf("error bin file could not be opened: \n");
        exit(1);
    }
        int i = 0;
        //while bin has data left read in data the size of struct Airline
        while(fread(&air[i], 33, 1, bin)==1){
    

                i++;
                numFlights++;
       }
       fclose(bin);
       //use library function qsort to sort array of structs by date using compareDates function below
       qsort(air,15,sizeof(Airline),compareDates);
       //loop through the sorted array of Airline structs, create a filename open the file and write each airline into file.
       for(int q = 0; q < numFlights; q++){
         makeFileName(air[q]->code, outdirectory);
                 if((out = fopen(file_path,"a"))== NULL){
                        printf("error output file could not be opened: \n");
                        fclose(bin);
                         exit(1);
            }
       fprintf(out,"%s %s %s %s %s\n", air[q]->code,air[q]->origin,air[q]->destination,air[q]->date,air[q]->time);
       
        fclose(out);
        //reset value for file_path
        memset(file_path,0,100);
    
}
    
    free(air);
    return 0;
}

int compareDates(const void *a,const void *b){
    
        
        const struct Airline *airA = a;
        const struct Airline *airB = b;
        if(strcmp(airA->date,airB->date) == 0){
            return strcmp(airA->time,airB->time);
        }
        else
        return strcmp(airA->date, airB->date);
        

    }
    
            
            

char * makeFileName(char * aircode, char * outdirectory){
 //set up file name to be created for airline
            
            char file_name[20];
//First value of FlightCode is always a letter so set filename[0] to the first value of aircode
            file_name[0] = aircode[0];
            //if aircode is not a capital letter (between 64 and 90 ACII then end the naming process and finish with .txt)
            if(aircode[1]>'Z' || aircode[1]<'A'){
                file_name[1] = '.';
                file_name[2] = 't';
                file_name[3] = 'x';
                file_name[4] = 't';
                file_name[5] = '\0';
            }
            else{
                file_name[1] = aircode[1];
                file_name[2] = '.';
                file_name[3] = 't';
                file_name[4] = 'x';
                file_name[5] = 't';
                file_name[6] = '\0';
            }
            
            
            strcat(file_path,outdirectory);
            strcat(file_path,"/");
            strcat(file_path,file_name);

            return file_path;

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


