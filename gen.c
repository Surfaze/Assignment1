#include <stdio.h>
void main(void){
	FILE* fp;
	fp = fopen("test.bin","wb");
	int str[] = {1,2,3,4};
	fwrite(str,1,sizeof str,fp);
	fclose(fp);
}