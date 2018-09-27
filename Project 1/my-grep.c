#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

       int
       main(int argc, char *argv[])
       {
           FILE *stream;
           char *line = NULL;
           size_t len = 0;
           ssize_t nread;
	   char searchterm[100];

           if (argc < 2) {
               fprintf(stderr, "%s: Searchterm[file...]\n", argv[0]);
               exit(1);
           }
	   if(argc ==2){
		char statement[100];
		fgets (statement,40,stdin);

		strcpy(searchterm, argv[1]);
		if(strstr(statement,searchterm) != NULL){
			printf("%s",statement);
		}
		exit(0);
	   }
	for(int i=2;i<argc;i++){
           stream = fopen(argv[i], "r");
           if (stream == NULL) {
		printf("my-grep: cannot open file\n");
		 perror("fopen");
               exit(1);
           }
	   
	   strcpy(searchterm, argv[1]);
           while ((nread = getline(&line, &len, stream)) != -1) {

		if(strstr(line,searchterm) != NULL){
		     if(argc ==3){
			printf("%s",line);
			}
		     else{
			printf("%s: %s",argv[i],line);
			}
		}
		 
           }
	}
           free(line);
           fclose(stream);
           exit(0);
       }


