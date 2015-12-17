#include <stdio.h>

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
	printf("%d",ret_code);

		puts("Array read successfully, contents: ");
		for(int n = 0; n < size; ++n) printf("%X ", in[n]);
		putchar('\n');

	   if (feof(fp))
			printf("Error reading %s: unexpected end of file\n",name);
	   else if (ferror(fp)) 
			printf("Error reading %s", name);
	
}