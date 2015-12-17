#include <stdio.h>
#include <stdlib.h>

void encryptFile(char* name);

void main(void){
	encryptFile("test.bin");
}

void encryptFile(char* name){
	
	FILE* fp;
	char* in;
	fp = fopen(name,"rb");
	fseek(fp,0,SEEK_END);
	size_t size = ftell(fp);
	rewind(fp);
	in = (char*)malloc(size);
	size_t ret_code = fread(in, sizeof *in, size, fp); 
	printf("%d\n",size);
	printf("%d\n\n",ret_code);

	if(ret_code == size){
		printf("Array read successfully, contents: \n");
		for(int n = 0; n < size; ++n) printf("%d ", in[n]);
		printf("\n");
	}
   if (feof(fp)){
		printf("Error reading %s: unexpected end of file\n",name);
		main();
   }
   else if (ferror(fp)) {
		printf("Error reading %s", name);
   }
}
