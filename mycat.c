#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
int main(int argc, char *argv[]) {
if (argc >=2){
   int i;
   for(i=1;i<argc;i = i+1){
	FILE *fp = fopen(argv[i],"r");
	if(fp == NULL){
	  printf("Cannot open file %d, %s \n",i,argv[i]);
	  exit(1);

	}else
	{
	  char buffer[1000];
	  printf("Printing %s #", argv[i]);
	  printf("_________________________________\n");
	  while(!feof(fp)){
		//reading the line and making sure it's successful
		if(fgets(buffer,1000,fp) != NULL){
			printf("%s",buffer);
		}
	   }
	   printf("Done with %s #", argv[i]);
	   printf("________________________________\n\n");
	 
	}
   }
   return 0; 
}else{
   printf("No file passed");
   exit(0);
}


}
