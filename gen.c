#include <stdio.h>
void main(void){
	FILE* fp;
	fp = fopen("test.bin","wb");
	char str[] = {'A',' ','C'};
	fwrite(str,1,sizeof(str),fp);
	fclose(fp);
}