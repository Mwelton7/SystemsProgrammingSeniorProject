#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <assert.h>
#define MAX 100
#define hashtablesize 1024
//*****READ ME IF INTERFACE DOES NOT WORK******
//type: gcc srchindx.c
//./a.out flag airport index.txt directory(or path to file)
//also works without index.txt as an argument
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
struct Element{
	struct node *data;
	int hashedKey;
};
struct node {
	int count;
	int hashedKey;
	char origin[4];
	struct node *next;
};
struct treenode{
	int count;
	char airline[2];
	struct treenode *left, *right;
};
struct node *head = NULL;
struct node *current = NULL;
struct Element* hashTable[hashtablesize];
struct Element* element;
void insertTable(int hashedKey, struct node *data);
void printTable();
void makeHashtable(char *directoryname,char *terms);
struct treenode *makeBst(char *directoryname,char *terms,struct treenode* root);
void insertList(int count, char *origin);
void inorderTraverse(struct treenode *root);
struct treenode *makeNode(char *airline,int count);
struct treenode *insert(struct treenode* node,char *airline,int count);
int main(int argc, char* argv[]) {
	char * invertedIndexFilename;
	char * directoryname;
	char * terms;
	char * flag;
	int hashkey;
	

	
	if(argc < 4){
		printf("Error wrong number of arguments. Not enough.\n");
		exit(1);
	}
	if(argc > 5){
		printf("Error wrong number of arguments. Too many.\n");
		exit(1);
	}
//set invertedIndexfilename to invind.txt if the argument is missing
	if(argc == 4){
		flag = (char *)malloc(strlen(argv[1])*sizeof(char));
		terms = (char *)malloc(strlen(argv[2])*sizeof(char));
		invertedIndexFilename = (char *)malloc(12*sizeof(char));
		directoryname = (char *)malloc(strlen(argv[3])*sizeof(char));
		strcpy(flag,argv[1]);
		strcpy(terms,argv[2]);
		strcpy(invertedIndexFilename,"invind.txt");
		strcpy(directoryname,argv[3]);

}
	if(argc == 5){
  		flag = (char *)malloc(strlen(argv[1])*sizeof(char));
		terms = (char *)malloc(strlen(argv[2])*sizeof(char));
		invertedIndexFilename = (char *)malloc(strlen(argv[3])*sizeof(char));
		directoryname = (char *)malloc(strlen(argv[4])*sizeof(char));
		strcpy(flag,argv[1]);
		strcpy(terms,argv[2]);
		strcpy(invertedIndexFilename,argv[3]);
		strcpy(directoryname,argv[4]);
		
	}
	

//if the directory name given is a text file handle the BST and Hashtable functions with just the one file as input.
	if(strstr(directoryname,".txt")!=NULL){
		int bstcount = 0;
		int hashcount = 0;
		FILE *in;
		struct Airline air;
		char airline[20];
    	memset(airline,0,20);
    	struct treenode *root = (struct treenode *)malloc(sizeof(struct treenode));
			if((in = fopen(directoryname,"r"))== NULL){
                        printf("error given file not opened expecting full path. \n");
                         exit(1);
            }
           while(!feof(in)){
        	
          //scan in one airport struct at a time
          //set the airline code to AA or F accordingly for the bst key
          fscanf(in,"%s %s %s %s %s",air.code,air.origin,air.destination,air.date,air.time);
          	airline[0] = air.code[0];
          	airline[1] = air.code[1];
          	
          	if(airline[1]<'A' || airline[1]>'Z'){
          		airline[1] = '\0';
          	}
          	else{
          		airline[2] = '\0';
          	}
          	//count the number of times the airport we are searching for occurs as an origin or destination
          if(!feof(in)){
          	
          	if(strcmp(air.destination,terms)==0){

          		bstcount++;
          	}
          	if(strcmp(air.origin,terms)==0){
          		hashcount++;
          	}
      }
         
        
      }
      //if one or more destination flights were found insert into the bst
      if(bstcount > 0){
      			struct treenode *root = (struct treenode *)malloc(sizeof(struct treenode));
				root = NULL;
				root =insert(root,airline,bstcount);
				//if the command is searching for destinations perform inorder traversal of bst
				if(strcmp(flag,"-d")==0 ){
				inorderTraverse(root);
				free(root);
			}
				
		}
		//if one or more origin flights were found insert into the hashtable
		if(hashcount > 0){
			insertList(hashcount,terms);
			//if the command is searching for origin flights print the contents of the hashTable
			if(strcmp(flag,"-o")==0 ){
				printTable();
		
			}
		
		}
		
    			
	}

		//if the directory that was given is a directory and not a file perform a search on all of the text files in given directory
		if(strstr(directoryname,".txt")==NULL){
		struct treenode *root = (struct treenode *)malloc(sizeof(struct treenode));
				root = NULL;
				root = makeBst(directoryname,terms,root);
				makeHashtable(directoryname,terms);
				//if the command is searching for origin flights print the contents of the hashTable
				if(strcmp(flag,"-o")==0 ){
				printTable();
		
			}
				//if the command is searching for destinations perform inorder traversal of bst
				if(strcmp(flag,"-d")==0 ){
				inorderTraverse(root);
				free(root);
			}
				
    			
		}
			

	

//free all of the memory allocated throughout program
	free(element);
	free(flag);
	free(terms);
	free(invertedIndexFilename);
	free(directoryname);
	return 0;

}

//function that accepts a directory name, terms to search for, and a treenode
//this function opens every file in the given directory in a while loop
//reads through them one line at a time using fscanf
//stores the lines in an aircode struct and compairs the destination strings with the term we are searching for
//counts them and inserts a node for every individual airline as the key and the number of flights heading to the airport in question
//it returns the root treenode after all the text files have been read.
struct treenode *makeBst(char *directoryname,char *terms,struct treenode* root){
FILE *input_file;
DIR *dp = NULL;
struct dirent *dptr = NULL;
char cwd[256];
struct Airline air;


if (NULL == (dp = opendir(directoryname)))
{
    printf("Cannot open the given directory %s", directoryname);
    exit(1);
}

    while((dptr = readdir(dp))!=NULL)
    {
    	int count = 0;
    	char airline[2];
    	memset(airline,0,2);
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
          	strcpy(airline,dptr->d_name);
          	if(airline[1]=='.'){
          		airline[1] = '\0';
          	}
          	airline[2] = '\0';
          if(!feof(input_file)){
          	
          	if(strcmp(air.destination,terms)==0){

          		count++;
          	

          }
      }
         
        
      }
      if(count > 0){

				root =insert(root,airline,count);
}
      fclose(input_file);  //close input after while loop for file
      
    }
    
   closedir(dp);
   return root;
   

}

//function that accepts a directory name and terms to search for
//this function opens every file in the given directory in a while loop
//reads through them one line at a time using fscanf
//stores the lines in an aircode struct and compairs the origin strings with the term we are searching for
//counts them and inserts a linked list node into the hashtable at the derived key generated by the hashfunction provided
//see insertList{}
void makeHashtable(char *directoryname,char *terms){
FILE *input_file;
DIR *dp = NULL;
struct dirent *dptr = NULL;
char cwd[256];
struct Airline air;
int count = 0;

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
          	if(strcmp(air.origin,terms)==0){
          		count++;
          	

          }
      }
         
        
      }

      fclose(input_file);  //close input after while loop for file
      
    }
    
   closedir(dp);
    //call insertList to populate count of airports
    insertList(count,terms);

}
//Provided hashFunction to generate keys
int  hashFunction(const char *s,  int T) {

   /* The parameter s represents the symbol to be hashed and  */
   /* the parameter T represents the size of the hash table.  */
   /* The function returns the hash value for the symbol s.   */

   /* String s is assumed to be terminated with '\0'.         */
   /* It is also assumed that T is at least 2. The returned   */
   /* hash value is an integer in the range 0 to T-1.         */

   /* The function computes the hash value using arithmetic   */
   /* based on powers of the BASE value defined below.        */

   #define  BASE   127

   int h = 0;     /* Will hold the hash value at the end. */
   int temp;      /* Temporary.                           */

   /* The hash value is computed in the for loop below. */
   for (;  *s != 0;  s++) {
       temp = (BASE * h + *s);
       if (temp < 0) temp = -temp;
       h = temp % T;
   }

   /* The hash value computation is complete. So, */
   return h;

} /* End of hash function */
//loop through hashTable array and when an index with nodes exists print entire linked list node contents to stdout
void printTable(){
	struct node *ptr;
	for(int i = 0; i<hashtablesize; i++){
		if(hashTable[i]!= NULL){
			ptr = hashTable[i]->data;
			while(ptr != NULL){
		printf("FROM HASHTABLE: Origin: %s Count: %d\n",ptr->origin,ptr->count);
		ptr = ptr->next;
			
		}
	}
}
}
void insertList(int count, char *origin){
	int hashedKey = 0;
	//create new linked list node to store element O (from project description)
	struct node *link = (struct node*)malloc(sizeof(struct node)); 
	//compute y = h(k) where k is origin string and hashedKey will be used for index of hashTable
	hashedKey = hashFunction(origin,hashtablesize);
	link->count = count;
	link->hashedKey = hashedKey;
	strcpy(link->origin,origin);
	//store information in node struct and assign next to the head
	//set to current head
	link->next = head;
	head = link;
	//call insertTable function with computed hashed index and new node to store
	insertTable(hashedKey,link);
}
void insertTable(int hashedKey,struct node *data){
	//create temporary element struct to use to store in hashTable array
	struct Element *element = (struct Element*) malloc(sizeof(struct Element));
	//set the temporary data element to newly created node struct
	element->data = data;
	element->hashedKey = hashedKey;
	//iterate over hashTable making sure there is room
	while(hashTable[hashedKey]!=NULL && hashTable[hashedKey]->hashedKey!= -1){
		++hashedKey;
		hashedKey %= hashtablesize;
	}
	//store the element at the derived hashedkey
	hashTable[hashedKey] = element;

}
//function to create a new treenode
//returns a struct treenode
//allocate memory using malloc and store the airline code and count being argued in temp treenode
//set left and right to NULL and return temp
struct treenode *makeNode(char *airline,int count){
	struct treenode *temp = (struct treenode*)malloc(sizeof(struct treenode));
	temp->count = count;
	strcpy(temp->airline,airline);
	temp->left = temp->right = NULL;
	return temp;
}
//function to insert a treenode with corresponding airline and count
//returns a struct treenode
//if the node doesn't exist call makeNode and return
//if the string for airline at current node is smaller than the airline being argued, insert to the left child
//and vise versa for if the string is larger (for constructing the tree alphabetically)
struct treenode *insert(struct treenode* node,char *airline,int count){
	if(node == NULL)
		return makeNode(airline,count);
	if(strcmp(airline,node->airline) < 0){
		node->left = insert(node->left,airline,count);
	}
	else if(strcmp(airline,node->airline) >0){
		node->right = insert(node->right,airline,count);
	}
	return node;
}
//function to perform an inorderTraversal printing to stdout
void inorderTraverse(struct treenode *root){
	if(root!= NULL){
		inorderTraverse(root->left);
		printf("BST Traversal: Aircode: %s Count: %d\n",root->airline,root->count);
		inorderTraverse(root->right);
	}
}