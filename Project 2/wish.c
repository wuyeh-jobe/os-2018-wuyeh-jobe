#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>


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
    // in real code you would check for errors in malloc here
    memcpy(result, s1, len1);
    memcpy(result + len1, s2, len2 + 1); // +1 to copy the null-terminator
    return result;
}



    int
    main(int argc, char *argv[])
    {
		FILE *stream;
	    char *line = NULL;
	 	size_t len = 32;
	    ssize_t nread;

		if (argc == 1){
	        while(1){
				printf("%s", "wish> ");
				/* Get input from command line*/
				getline (&line,&len,stdin);
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

			    /* Compare the two strings provided */
			    if(strcmp(arg[0], "exit") == 0){
					exit(0);
				}
				else if (strcmp(arg[0], "cd") == 0)
				{
					
				   if (count==2){
				   	if (chdir(arg[1])==0){
				   		printf("%s\n","in folder now");
				   	}
				   	else{
				   		printf("%s\n","Folder not available");
				   	}

				   }
				   else{
				      	printf("%s\n", "Error: cd takes only one argument");
				   }
				}
				else{
					//Array to be passed to execv
				    char *array[10];
				    //Fill above array with nulls
				    for (int i = 1; i<11;i++){
				    	array[i]=NULL;
				    }
					char *path1 = concat("/usr/bin/",arg[0]);
					char *path2 = concat("/bin/",arg[0]);
					int flag = 0;
					if(access(path1,X_OK)==0){
						printf("%s\n", "accessed 1");
						flag = 1;
					}
					if(access(path2,X_OK)==0){
						printf("%s\n", "accessed 2");
						flag = 2;
					}
					//Put arguments in array for execv
					if(count>1){
						for(int i=1;i<count;i++){
							array[i] = arg[i];
						}
					}
					

					int rc = fork();

					if (rc==0){
						printf("%d\n", flag);
						if (flag == 1){
							array[0] = path1;
							execv(path1, array);
						}
						else if(flag == 2){
							array[0] = path2;
							execv(path2, array);
						}
						
						printf("%s\n", "Here you go!");
					}
					else
					{
						wait(NULL);
						//printf("hello, I am parent of %d (rc_wait:%d) (pid:%d)\n", rc, rc_wait, (int) getpid());
					}

				}

				
		   }
		    
		}
		else if(argc == 2){
			stream = fopen(argv[1], "r");
			if (stream == NULL) {
			    printf("my-grep: cannot open file\n");
			    perror("fopen");
			    exit(1);
			 }
			 while ((nread = getline(&line, &len, stream)) != -1) {
			    printf("%s", line);		
			 }//end of the while loop
		}//else if ends here
		else{
			printf("%s\n", "Error: Only one or no argumennt allowed");	
		}
		return 0;
    }


