#include <stdio.h>
#include <stdlib.h>
int main(int argc, char *argv[]) {
if (argc >=2){
   int i;
   for(i=1;i<argc;i = i+1){
	FILE *fp = fopen(argv[i],"r");
	if(fp == NULL){
	  printf("my-cat: Cannot open file %d, %s \n",i,argv[i]);
	  perror("fopen");
	  exit(1);

	}else
	{
	  char buffer[1000];
	  while(!feof(fp)){
		//reading the line and making sure it's successful
		if(fgets(buffer,1000,fp) != NULL){
			printf("%s",buffer);
		}
	   }
	 
	}
	fclose(fp);
   }
   
   return 0; 
}else{
   printf("No file passed \n");
   exit(0);
}


}
