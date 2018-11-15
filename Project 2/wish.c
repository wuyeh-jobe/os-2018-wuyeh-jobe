#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <pthread.h> 

void remove_newline_ch(char *line)
{
    int new_line = strlen(line) -1;
    if (line[new_line] == '\n')
        line[new_line] = '\0';
}

char* concat(const char *s1, const char *s2)
{
    const size_t len1 = strlen(s1);
    const size_t len2 = strlen(s2);
    char *result = malloc(len1 + len2 + 1); // +1 for the null-terminator
    // in real co de you would check for errors in malloc here
    memcpy(result, s1, len1);
    memcpy(result + len1, s2, len2 + 1); // +1 to copy the null-terminator
    return result;
}

void error(){
	char error_message[30] = "An error has occurred\n";
    write(STDERR_FILENO, error_message, strlen(error_message)); 
}

char *paths[10];
int pathsCount = 2;
int pathInvoked = 0;
void *executeCommands(void *l){

    if(pathInvoked==0){
		paths[0] = "/bin";
		paths[1] = "/usr/bin";
	}
	
    char *line = (char *) l;
   
    //------------------
	//Remove new line chracter using defined funtion above
	remove_newline_ch(line);			
	//stores space, used as break point
	const char s[2] = " ";
	//Array to store arguments
	char *arg[10];
	//stores tokens from strings
	char *token;
   /* get the first token */
   token = strtok(line, s);
   int count = 0;
	/* get tokens and add them into the array */
   while( token != NULL ) {
      arg[count] = token;
      count = count+1;
      token = strtok(NULL, s);
   }
	if (strcmp(arg[0], "path") == 0){
		pathsCount = 0;
		pathInvoked = 1;
		for(int j = 1;j<count;j++){
				paths[j-1] = malloc(strlen(arg[j])+1);
				strcpy(paths[j-1], arg[j]);
				pathsCount = pathsCount +1;
		}	
	}
    /* Compare the two strings provided */
    else if(strcmp(arg[0], "exit") == 0){
		exit(0);
	}
	else if (strcmp(arg[0], "cd") == 0)
	{
	   if (count==2){
	   	if (chdir(arg[1])==0){
	   		printf("%s\n","in folder now");
	   	}
	   	else{
	   		error();
	   		//printf("%s\n","Folder not available");
	   	}
	   }
	   else{
	   		error();
	      	//printf("%s\n", "Error: cd takes only one argument");
	   }
	}
	else{
		//Array to be passed to execv
	    char *array[10];
	    //Fill above array with nulls
	    for (int i = 1; i<11;i++){
	    	array[i]=NULL;
	    }		
		int redirect = 0; //For checking if the redirect symbol is found
		char *filename; // get the filename if it's found
		//Put arguments in array for execv
		if(count>1){
			for(int i=1;i<count;i++){
				if (strcmp(arg[i], ">") != 0){
					array[i] = arg[i];
				}
				else{
					redirect = 1;
					i++;
					filename = arg[i];
				}
			}
		}
		int rc = fork();
		if (rc==0){
			for (int k = 0;k<pathsCount;k++){
				char *pathC = concat(paths[k],"/");
				char *path = concat(pathC,arg[0]);
				array[0] = path;
				if (access(path,X_OK)==0){

					//redirect output to the file

					if (redirect == 1){
						FILE* file = fopen(filename, "w+");
					   dup2(fileno(file), fileno(stdout));
					   dup2(fileno(file), fileno(stderr));

					   fclose(file);
					}
					execv(path, array);
				}
			}				
			//printf("%s\n", "Specify the path!");
			error();
		}
		else
		{
			wait(NULL);
		}
	}
}

int main(int argc, char *argv[]) 
{ 
		FILE *stream;
		char *line = NULL;
		size_t len = 32;
    	ssize_t nread;
    	if (argc==1)
    	{
    		while(1){
				printf("%s", "wish> ");
				/* Get input from command line*/
				getline (&line,&len,stdin);
				//Remove new line chracter using defined funtion above
				remove_newline_ch(line);	
				const char s[2] = "&";
				//Array to store arguments
				char *commands[10];
				//stores tokens from strings
				char *token;
			   /* get the first token */
			   token = strtok(line, s);
			   int count = 0;

				/* get tokens and add them into the array */
			   while( token != NULL ) {
			      commands[count] = token;
			      //printf("%s\n", commands[count]);
			      count = count+1;
			      token = strtok(NULL, s);
			   }
			   //printf("%d\n", count);
			   	int i,j; 
				pthread_t thread_id[count]; 

				 for (i = 0; i < count; i++){
				 	pthread_create(&thread_id[i], NULL, executeCommands, commands[i]); 
				 	
				 }

				   for(j=0; j < count; j++)
				   {
				      pthread_join( thread_id[j], NULL);
				   }  		
			}
		}
		else if(argc ==2){
			stream = fopen(argv[1], "r");
			if (stream == NULL) {
				error();
			    exit(1);
			 }
			 while ((nread = getline(&line, &len, stream)) != -1) {		
			    remove_newline_ch(line);
			    const char s[2] = "&";
				//Array to store arguments
				char *commands[10];
				//stores tokens from strings
				char *token;
			   /* get the first token */
			   token = strtok(line, s);
			   int count = 0;

				/* get tokens and add them into the array */
			   while( token != NULL ) {
			      commands[count] = token;
			      count = count+1;
			      token = strtok(NULL, s);
			   }
			   	int i,j; 
				pthread_t thread_id[count];
				 for (i = 0; i < count; i++){
				 	pthread_create(&thread_id[i], NULL, executeCommands, commands[i]); 				 	
				 }		
				for(j=0; j < count; j++)
				   {
				      pthread_join( thread_id[j], NULL);
				   } 

			 }//end of the while loop
		}
		else{
			error();
			//printf("%s\n", "Only two arguments are allowed");
		}

	 return 0; 
} 

	



