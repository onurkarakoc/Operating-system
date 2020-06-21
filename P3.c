#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <errno.h>
#include <sys/types.h>
#include<sys/dir.h>
#include<dirent.h>
#include<fcntl.h>
#include <limits.h>
#include <malloc.h>
#include <sys/stat.h>
#define maxSize 10
#define size 100

// Struct for list nodes
typedef struct lst_node_s {
char data[100];
struct lst_node_s* next;
}listnode; 
// listnode has properties of lst_node_s

listnode * listroot = NULL;
listnode * listending = NULL;

void changeDirectory(char **argv);
void initShell();
void dir();
void execute(char **argv, int *j);
void List_enqueue(char data[size]);
void History();
int checkifexecutable(const char *isExe);
int findpathof(char *pth, const char *exe);
void findloc(char **argv);
int parsecmd(char* cmd, char** params);
int execpipe (char ** argv1, char ** argv2);


void initShell(){
    char  line[100];             /* the input line                 */
    char  *argv[10];              /* the command line argument      */
    int  *j =0 ;                   // j is argument number for argv

        while (1) {                   /* repeat until done ....         */
            printf("myshell>");     /*   display a prompt             */
                gets(line);	/*   read in the command line     */ 
        	List_enqueue(line);     /* queues the entered arguments*/         
                j=parsecmd(line, argv);       /*   parse the line    */
		execute(argv,j); /* calls execute function after parsed the argument*/
        }
}



void List_enqueue(char data[size]){
    if(listroot == NULL){ // List queue is empty
        listroot = malloc(sizeof(listnode));
        for(int i=0;i<strlen(data);i++){

            listroot ->data[i] = data[i]; //moves the argument characters to the array
        }
        listroot -> next = NULL; //moves until find NULL
        listending = listroot;
    }else { // If queue is not empty , add to queue.
        listending -> next = malloc(sizeof(listnode));
        for(int i=0;i<strlen(data);i++){

            listending->next ->data[i] = data[i];//moves the next argument characters to the array
        }
        listending = listending -> next ; //moves next block
        listending -> next = NULL; //moves until find NULL
   
	}
}

void History(){
    listnode * temp = listroot; //assignning temp to the head of queue
    int index = 1;
    while (temp!=NULL && index < 11){
        printf("[%d] %s\n",index, temp -> data);	// prints the array values that are pointed by temp(listroot)
        temp = temp->next;//moves to next block
        index++;
       
    }
}

/*Function parse() takes an input line and returns a zero-terminated array of char pointers. This function loops until a zero is found, which means the end of the input line is found. If the current character of line is not a zero, parse() skips spaces and replaces them with zeros. Once parse() finds a not-space character, it saves the location to the argv and index. Then, parse() skips all not-space characters. This process repeats until find the end of the string and sets argv to zero.
*/


void changeDirectory(char **argv){
    
	int result = 0;

    if (argv[2] != NULL && argv[1]!=NULL){//checks if the second and third argument is NULL or not
        printf("%s: Too many operands \nUsage: %s <pathname>\n", argv[0],argv[0]);//prints error message for entering too many arguments
    }
    else {
        if(argv[1] == NULL){//checks if the second argument is NULL
            const char* home = getenv("HOME"); //assigning home to $HOME
            chdir(home);//changes directory to home directory
        }
        else{
            result = chdir(argv[1]);//assigns the result to second argument 
            if (result == -1){
                printf("%s%s\n","No Such Directory: ",argv[1]);// if second argument is not existing then prints error
            }
        }
    }
   
}

void dir(){
   
	 char cwd[1024];
       		 getcwd(cwd, sizeof(cwd)); //gets the current directory 
        	printf("%s\n",cwd);
}


 
 /*
Return non-zero if the name is an executable file, and
 zero if it is not executable, or if it does not exist.
 */

int checkifexecutable(const char *isExe)
{
     int result;
     struct stat statinfo; /*to initiliaze the functions inside struct which are we using;


struct stat {
              
               mode_t    st_mode;        
               uid_t     st_uid;         
               gid_t     st_gid;         
*/
    
     result = stat(isExe, &statinfo); // assigning the variable to stat function that takes a pathname(argum.) and a stat struct pointer . On succes 0 is returned , otherw. -1
//S_ISREG evaluates if the argument(The st_mode member in struct stat) is a regular file or not 
     if (result < 0) return 0;
     if (!S_ISREG(statinfo.st_mode)) return 0;//checks whether the file type is regular or not st_mode : file type!!

     if (statinfo.st_uid == geteuid()) return statinfo.st_mode & S_IXUSR; // checks whether the user ID of the file is equal with the get function if it is then it returns the search value and the file type non-zero or zero
     if (statinfo.st_gid == getegid())   return statinfo.st_mode & S_IXGRP;// checks whether the group ID of the file is equal with the get function, if it is then it returns the search value and file type non-zero or zero
     return statinfo.st_mode & S_IXOTH;//returns the file type non-zero or zero and search value
}

/*
 
  Find executable file by searching in the PATH environment variable.
 
  const char *exe - executable name to search for.
        char *pth - the path found is stored here.
 
  If a path is found, returns non-zero, and the path is stored
  in pth.  If exe is not found returns 0, with path undefined.
 */

int findpathof(char *pth, const char *exe)
{
     char *searchpath;
     char *begining, *end;
     int stop, found;
     int len;

     if (strchr(exe, '/') != NULL) {//checks whether the exe is NULL or not
      if (realpath(exe, pth) == NULL) return 0;//checks if the path of executable's absolute path NULL or not
      return  checkifexecutable(pth); //returns the path is non-zero or zero 
     }

     searchpath = getenv("PATH");// assigning $PATH enviroment
     if (searchpath == NULL) return 0; //checks whether variable wih path is NULL 
     if (strlen(searchpath) <= 0) return 0;//takes the variables length checks is it smaller then zero 

     begining = searchpath;//assigning the path to a variable
     stop = 0; found = 0;
    do { //while it stops and find the value 
      end = strchr(begining, ':');//assigns the variable with a ":" to search in the path, karakterin bulunduğu konumu döner.
      if (end == NULL) { //if variable is NULL 
           stop = 1;
           strncpy(pth, begining, PATH_MAX);//copies the path characters to the variable with number of PATH_MAX
           len = strlen(pth);//takes length of the path and assigns it to a variable 
      } else {
           strncpy(pth, begining, end - begining);//copies the path end-begining  number of characters to the variable 
           pth[end - begining] = '\0'; //assigns the end-begining's number of path array to and null-terminated character
           len = end - begining; //assigns the variable
      }
      if (pth[len - 1] != '/') strncat(pth, "/", 1); //checks whether the path of array is equal to concat
      strncat(pth, exe, PATH_MAX - len); 
      found = checkifexecutable(pth);
      if (!stop) begining = end + 1;
     } while (!stop && !found);
     
     return found;
}

void findloc(char **argv){//finds the executable files location
    
     char path[PATH_MAX];
     char *progpath = strdup(argv[0]); // assigning a pointer to the duplicate of first argument
     char *prog = basename(progpath); // assigning a pointer break a pathname string into directory components.
     char *exe;
     	exe = argv[1]; //assigning the executable to a variable

     		if (!findpathof(path, exe)) { //calls the function with parameters executable file and the path 
     			printf("No executable %s\n found", exe);
    		 }		
     puts(path);//writes the path without null character
     free(progpath);//frees the memory allocation to not have a memory leak because of the basename function
}

void execute(char **argv, int *j){//checks the contidions or arguments from the user and calls function inside

  pid_t  pid; //assigning a pid for fork 
  int background;//holds the background variable to check whether ampersand entered or not
  int codewait;//waitpid function is assigned for it in the above
  int status; // in fork
  int k, y, x;    
  int f = 1; //flag selects code block's way 
  char * argv1[maxSize+1] = {0};   // we will seperate one array to 2 array for pipe. 
  char * argv2[maxSize+1] = {0};



        if(strcmp(argv[0], "dir") == 0){//checks the argument and compares it with the related string
            dir(); // calls the dir function
           
        }
        else if(strcmp(argv[0], "cd") == 0){//checks the argument and compares it with the related string
            changeDirectory(argv); // calls the change directory function
           
        }
        else if(strcmp(argv[0], "history") == 0){//checks the argument and compares it with the related string
            History(); // calls the history function
        }
        else if (strcmp(argv[0], "findloc")==0){//checks the argument and compares it with the related string
            findloc(argv);    // calls the find location function
        }
	else if (strcmp(argv[0], "bye") == 0){  /* checks whether it is bye or not    */
                       exit(0);       
        }
	else if (strcmp(argv[0], "") ==0){  //empty case
			printf("%s\n", "INVALID");
	}
	else {
		for (k=0; k <j; k++) {  
            		if (strcmp(argv[k], "|") == 0) {   // for pipe
				
                		f = 0; y = k;      
               			printf("pipe found\n");
				return;
            		}               
        	}
				 
		if (f==0) {
            		for (x=0; x<k; x++) {    
               			argv1[x]=argv[x]; //seperating 2 vector
            		}     
           		int z = 0;     
            		for (x=k+1; x< j; x++) {     
                		argv2[z]=argv[x];
                		z++;
            		}     
            		execpipe(argv1, argv2);
		    
         } else if (f==1) {     
        	if ((pid = fork()) < 0) {     // fork a child process          
              		printf("*** ERROR: forking child process failed\n");
                	exit(1);
        	}
        	else if (pid == 0) {          // for the child process
              		if (execvp(*argv, argv) < 0) {     // execute the command  
                		printf("*** ERROR: exec failed\n");
                 		exit(1);
                 	}
             	}
             	else if(argv[1] != '&'){    //checks whether the input is ampersand (&)                         
            		int codewait=waitpid(pid,&status,NULL); // wait for completion  //foreground call function 
                		if(codewait<0){
                            		printf("Wait went wrong :<\n");
                        	}
				else{
                           	 printf("Wait done.\n");
				}

  
             	}

             	else{
             			
             		int codewait=waitpid(pid,&status,0); // wait for completion  //foreground call function 
                		if(codewait<0){
                            		printf("Wait went wrong :<\n");
                        	}
				else{
                           	 printf("Wait done.\n");
				}
          

		}
	}
}
}


int parsecmd(char* cmd, char** params) { //split cmd into array of parameters
    int i,n=-1;
    for(i=0; i<maxSize; i++) {
        params[i] = strsep(&cmd, " ");
        n++;
        if(params[i] == NULL) {break;}
    }
    return(n);
}


int execpipe (char ** argv1, char ** argv2) {
    int fds[2];
    pipe(fds);
    int i;
    pid_t pid = fork();
    if (pid == -1) { //error
        char *error = strerror(errno);
        printf("%s\n","error fork!!");
        return 1;
    } 
    else if (pid == 0) { // child process
        close(fds[1]);//for using write end
        dup2(fds[0], 0); //close(fds[0]);
        execvp(argv2[0], argv2); 
        char *error = strerror(errno);
        printf("%s\n","unknown command");
        return 0;
    } else { // parent process
        close(fds[0]);//for using read end
        dup2(fds[1], 1);//close(fds[1]);
        execvp(argv1[0], argv1); 
        char *error = strerror(errno);
        printf("%s\n","unknown command");
        return 0;
    }
	
}

void main(){
	initShell();}
