#include <stdio.h>
#include <stdlib.h>

void encryptFile(char* name){
	
	FILE* fp;
	char* in;
	fp = fopen(name,"rb");
	fseek(fp,0,SEEK_END);
	size_t size = ftell(fp);
	rewind(fp);
	in = (char*)malloc(size);
	size_t ret_code = fread(in, sizeof *in, 1, fp); 
	printf("%d\n",size);
	printf("%d\n",ret_code);

	printf("Array read successfully, contents: \n");
	for(int n = 0; n < size; ++n) printf("%X ", in[n]);
	printf("\n");

	   if (feof(fp))
			printf("Error reading %s: unexpected end of file\n",name);
	   else if (ferror(fp)) 
			printf("Error reading %s", name);	
}

void main(void){
	int choice;
	do{
		printf("Action:\n1. Encrypt Message\n2. Encrypt Binary File\n3. Exit\nYour choice: ");
		scanf("%d", &choice);
		//myFlush();
		printf("\n--------------------------------------------------------------\n");
	}while(choice < 1 || choice > 3);
	(choice == 1)? 0: (choice == 2) ? encryptFile("test.bin") :  (choice == 3) ? exit(0) : 0;
}