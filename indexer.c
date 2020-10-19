#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#define MAX 100
//*****READ ME IF INTERFACE DOES NOT WORK******
//type: gcc indexer.c
//./a.out index.txt pathname
//or: ./a.out pathname
struct Airline{
        char code[8];
        char origin[4];
        char destination[4];
        char date[11];
        char time[6];
        
    };

typedef struct Code{
    char aircode[4];
    char filename[6];
    int count;
}Code[MAX];


typedef struct CharArray{
    char aircode[4];

}CharArray[MAX];
typedef struct FileArray{
    char filename[6];

}FileArray[MAX];
int compareCodes(const void *a,const void *b);
int compareFrequency(const void *a,const void *b);
int sortIndex(Code *code,Code *finalcode,char * aircode,char * filename, int codeSize,int finalindex);
int ifContains(Code *dupArr,char * aircode,char * filename,int f);
int j = 0;
int codeSize = 0;
int dupSize = 0;

int main(int argc, char* argv[]) {
char * invertedIndexFilename;
char * directoryname;
	if(argc < 2){
		printf("Error wrong number of arguments. Not enough.\n");
		exit(1);
	}
	if(argc > 3){
		printf("Error wrong number of arguments. Too many.\n");
		exit(1);
	}
if(argv[2]!=NULL && argv[1]!=NULL){
invertedIndexFilename = (char *)malloc(strlen(argv[1])*sizeof(char));
directoryname = (char *)malloc(strlen(argv[2])*sizeof(char));
strcpy(invertedIndexFilename,argv[1]);
strcpy(directoryname,argv[2]);
}
if(argv[2]==NULL && argv[1]!=NULL){
  invertedIndexFilename = (char *)malloc(12 *sizeof(char));
  directoryname = (char *)malloc(strlen(argv[1])*sizeof(char));
  strcpy(invertedIndexFilename,"invind.txt");
  strcpy(directoryname,argv[1]);
}
DIR *dp = NULL;
struct dirent *dptr = NULL;
FILE *input_file;
FILE *out_file;
char c,cwd[256]; //created new character array
struct Airline air;

Code* code = (Code*)malloc(MAX * sizeof(Code));
Code* finalcode = (Code*)malloc(MAX * sizeof(Code));
Code* dupArr = (Code*)malloc(MAX * sizeof(Code));
CharArray* codeArr = (CharArray*)malloc(MAX * sizeof(CharArray));
FileArray* fileArr = (FileArray*)malloc(MAX * sizeof(FileArray));

if (NULL == (dp = opendir(directoryname)))
{
    printf("Cannot open the given directory %s", directoryname);
    exit(1);
}

    while((dptr = readdir(dp))!=NULL)
    {
        if(!strcmp (dptr->d_name, "."))
        {
            continue;
        }
        if(!strcmp (dptr->d_name, ".."))
        {
            continue;
        }
    if(getcwd(cwd, sizeof(cwd)) == NULL) //this is important
    {
        printf("No such file or directory");
        continue;
    }
    if(dptr->d_name[0]=='.')  //Files never begin with '.'
        continue;
    strcat(cwd,"/");   //For windows "\"
    strcat(cwd,directoryname);
    strcat(cwd,"/");   //For windows "\"
    strcat(cwd,dptr->d_name);
   

        
        input_file = fopen(cwd, "r");
        if(input_file == NULL)
        {
            printf("Cannot open file or no file exists\n");
            fclose(input_file);
            exit(1);
        }

        while(!feof(input_file)){
          //scan in one airport struct at a time
          fscanf(input_file,"%s %s %s %s %s",air.code,air.origin,air.destination,air.date,air.time);

          if(!feof(input_file)){
            //store origin in code[j]
          //initialize count to 1
          strcpy(code[j]->aircode,air.origin);
            strcpy(code[j]->filename,dptr->d_name);
            code[j]->count++;
            j++;
            //store destination in code[j+1]
            //initialize count to 1
            strcpy(code[j]->aircode,air.destination);
            strcpy(code[j]->filename,dptr->d_name);
            code[j]->count++;
            j++;
            //keep track of size of code
            codeSize = codeSize + 2;
        }
      }
      fclose(input_file);  //close input after while loop for file
      
    }
	
    //loop through the newly created Code code[] struct array and find any occurance where there are duplicate aircode,filename pairs 
    //copy the pairs into a new Code dupArr[] to hold the duplicate pairs
    //also check to make sure dupArr does not also wind up with duplicates with ifContains{}function implemented below
    //increment 
    for(int t = 0; t<codeSize; t++){
        
        for(int r = t+1; r<codeSize; r++){

          if(strcmp(code[t]->aircode,code[r]->aircode)==0 && strcmp(code[t]->filename,code[r]->filename)==0
           && 1 == ifContains(dupArr,code[r]->aircode,code[r]->filename,dupSize)){
                strcpy(dupArr[dupSize]->aircode,code[t]->aircode);
                strcpy(dupArr[dupSize]->filename,code[t]->filename);
                dupSize++;
                
               }
              }
          }

            //QSORT relies on a helper function to sort the array here we use compareCodes{}
            //sort the index array by aircode alphabetically to help with counting
          qsort(dupArr,dupSize,sizeof(Code),compareCodes);
            //then check code struct vs new index struct and populate code struct with correct counts.
         
          for(int i = 0; i<dupSize; i++){
            //flag counts how many times the if statement is true(how many duplicates there are) and overwrites the count in code[]
            int flag = 0;
            for(int j = 0; j <codeSize; j++){
              //if the aircode and filename are the same in code[] and index[] and the next aircode and filename are different increment flag and store in count
            if(strcmp(code[j]->aircode,dupArr[i]->aircode)==0 && strcmp(code[j]->filename,dupArr[i]->filename)==0){
              flag++;
              
              if(flag > 1){
                code[j]->count = flag;
                
              }
              else
                code[j]->count++;

            }
        }
      }

      //Make a separate list of each file being used and store in "fileArr[]" by looping over code[] and saving any new files we come across.
      int numFiles = 0;
      for(int t = 0; t<codeSize; t++){
        
        strcpy(fileArr[numFiles]->filename,code[t]->filename);
        if(strcmp(fileArr[numFiles]->filename,code[t+1]->filename)!=0){
          numFiles++;
                strcpy(fileArr[numFiles]->filename,code[t+1]->filename);

               }
              }
            //sort code[] by aircode alphabetically to help with cutting out duplicates see "sortIndex{}"
      qsort(code,codeSize,sizeof(Code),compareCodes);
      //Make a separate list of each aircode that occurs and store in struct array codeArr[] by looping over code[] and saving any new codes we come across.
      int numCodes = 0;
      for(int t = 0; t<codeSize; t++){
        
        strcpy(codeArr[numCodes]->aircode,code[t]->aircode);
        if(strcmp(codeArr[numCodes]->aircode,code[t+1]->aircode)!=0){
          numCodes++;
                strcpy(codeArr[numCodes]->aircode,code[t+1]->aircode);

               }
              }
              //sort code[] by count variable in order of most frequent flights
              qsort(code,codeSize,sizeof(Code),compareFrequency);
      
            //make one new final list of instances where there are no duplicate values by calling sortIndex
            //sortIndex is called using every iteration of aircode and filename in codeArr and fileArr
            //sortIndex returns the current index of new struct array "finalcode[]"
            int finalindex = 0;
          for(int i = 0; i <numCodes; i++){
            for(int j = 0; j < numFiles; j++){
      finalindex = sortIndex(code,finalcode,codeArr[i]->aircode,fileArr[j]->filename,codeSize,finalindex);

    }}
    //sort newest struct by count frequency for the assignment to get ready to print to file
    qsort(finalcode,finalindex,sizeof(Code),compareFrequency);
    //open the designated inverted index file and append
    if((out_file = fopen(invertedIndexFilename,"a"))== NULL){
                        printf("error out file not opened: \n");
                         return 0;
                     }
                 //nested loop for comparing each code in codeArr[](Every possible aircode) and the newly created finalcode[] that has been sorted by frequency
                 //look for a select aircode term (from codeArr) and writes the corresponding filename and count associated with each term
                 //the aircodes are sorted in ascending alphabetic order and the counts are in decending order as requested
            for(int i = 0; i <numCodes; i++){
              
              fprintf(out_file,"<list> %s\n",codeArr[i]->aircode);

              for(int j = 0; j < finalindex; j++){
          if(strcmp(finalcode[j]->aircode,codeArr[i]->aircode)==0){
            fprintf(out_file,"%s %d ",finalcode[j]->filename,finalcode[j]->count);
          }

    }
      fprintf(out_file,"\n</list>\n");
  }
    fclose(out_file);
    
    
    closedir(dp);
      //close dp after while readdp
    free(code);
    free(dupArr);
    free(codeArr);
    free(fileArr);
    free(finalcode);
    free(invertedIndexFilename);
    free(directoryname);
    
    
return 0;
}
  //compares first aircode to next aircode and sorts Alphabetically when qsort is called
     int compareCodes(const void *a,const void *b){
    
        
        const struct Code *codeA = a;
        const struct Code *codeB= b;
        
            return strcmp(codeA->aircode,codeB->aircode);
      
        

    }    
           int compareFrequency(const void *a,const void *b){
    
        
        const struct Code *fileA = a;
        const struct Code *fileB= b;
        
            return fileA->count < fileB->count;
      
        

    }

    //accepts two Code structs two char * and two integers
    //original *code[] to be compared and new *finalcode[] to store new list with no duplicates
    //char * aircode and filename are passed in from the loop above to test against code[]
    //int codeSize is the length of code[] and finalindex is returned to keep track of new struct array finalcode[]
    //if the aircode and filename in code[i] are the same as the ones being used as arguments then set a flag and copy the contents into new struct array finalcode[]
    //increment finalindex and return
      int sortIndex(Code *code,Code *finalcode,char * aircode,char * filename, int codeSize,int finalindex){ 
        int flag = 0;
        
          for(int i = 0; i<codeSize;i++){
            
            if(strcmp(code[i]->aircode,aircode)== 0 &&strcmp(code[i]->filename,filename)==0 && flag == 0){
            
              flag = -1;
              strcpy(finalcode[finalindex]->aircode,code[i]->aircode);
              strcpy(finalcode[finalindex]->filename,code[i]->filename);
              finalcode[finalindex]->count = code[i]->count;
              finalindex++;
              }
              
            }
              return finalindex;
            }
            //iterates over a Code *[] returns -1 if the array contains same aircode and filename as arguments at same index.
            //returns 1 if it does not contain the pair of strings
     int ifContains(Code *dupArr,char * aircode,char * filename,int f){
     		for(int i=0; i < f; i++){
     			if(strcmp(dupArr[i]->aircode,aircode)==0 && strcmp(dupArr[i]->filename,filename)==0){
     				return -1;
     			}
     		}
     		return 1;
          
      
         }
