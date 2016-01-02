#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//define Key struct so that i don't have to keep creating
typedef struct{
	int n,e,d;
} Keys;

typedef struct{
	long *array;
	size_t size;
} Array;

void myFlush();
void decrypt();
void decryptBin();
Keys readPrivKey();
char* getEncryptedFileName();
char* readEncrypted();
long bin_mod(long b, long e,long m);
void initArray(Array *a, size_t initialSize);
void freeArray(Array *a);
int* readBin(FILE* fp, int* arr, size_t* size, char* name);
void decryptBytes(int* arr, int size, Keys key);
void writeToFile(int* C,int size, char type);

void main (void){
	
	//menu for decrypting
	int choice;
	do{
		printf("Action:\n1. Decrypt Message\n2. Decrypt Binary File\n3. Exit\nYour choice: ");
		scanf("%d", &choice);
		myFlush();
		printf("\n--------------------------------------------------------------\n");
	}while(choice < 1 || choice > 4);
	(choice == 1)? decrypt() : (choice == 2)? decryptBin() : (choice == 3)? exit(0) : 0;
	
}

void myFlush(){
	
	while(getchar()!='\n');
}

Keys readPrivKey(){
	
	size_t size; //store the size of the stream of file, declare outside so others can use
	
	//get the private key file
	char name[100];
	printf("Name of private key file you would like to use(e.g. xxx_priv.txt): ");
	scanf("%s",name);
	myFlush();
	
	//open it
	FILE* fp;
	if(fp = fopen(name,"r")){ //open the file in read mode, if it exists do these
		
		
		//get total size of the stream
		fseek(fp, 0, SEEK_END); //seek the stream of fp, from o, to the end of stream
		size = ftell(fp); //stores how many character are in the sream, including the EOF, each chara is 1 byte
		
		rewind(fp); //rewind so won't get segment fault
		
		//using token, split the n and d of the Key
		char in[100];
		fread(in, 1, size, fp);
		const char tok[2] = ";";
		char* token = strtok(in, tok);
		Keys key;
		key.n = atoi(token);
		token = strtok(NULL, tok);
		key.d = atoi(token);
		fclose(fp);
		return key;
		
		
	}else{
		printf("\nError reading file \"%s\"\n", name);
		readPrivKey();
	}
}

char* getEncryptedFileName(){
	//get encrypted file
	static char name[100]; //need to defind as static as C don't allow the return of the address of a local variable to outside of the function
	printf("Name of encrypted file you would like to decrypt(e.g. abc.txt): ");
	scanf("%s",name);
	myFlush();
	
	FILE* fp;
	
	if(fp = fopen(name, "r")){
		fclose(fp);
		return name;
	}else{
		printf("Error reading file \"%s\"\n",name);
		fclose(fp);
		getEncryptedFileName();
	}
	
}

void initArray(Array *a, size_t initialSize){
	a->array = (long*)malloc(initialSize * sizeof(long));
	a->size = initialSize;
	
}
void freeArray(Array *a){
	free(a->array);
	a->array = NULL;
	a->size = 0;
	
}


void decrypt(){
	
	//get keys
	Keys key = readPrivKey();
	char* name = getEncryptedFileName();
	
	//Read contents of encrypted file
	char temp[1000];
	
	FILE* fp;
	if( fp = fopen(name, "r")){
		fgets(temp, 1000, (FILE*)fp);
		
		//printf("print temp = %s\n",temp);
		
		const char tok[2] = ";";
		char* token = strtok(temp, tok);
		
		int numberOfChara;
		
		while(token != NULL){
			//printf("%s\n", token);
			token = strtok(NULL, tok);
			numberOfChara++;
		}
		
		
		//numberOfChara--; //because it will ++ 1 more time after while loop
		
		
		//init the array
		Array ctArray;
		initArray(&ctArray, numberOfChara);
		
		//populate
		rewind(fp);
		fgets(temp, 1000, (FILE*)fp);
		//printf("%s\n", temp);
		
		/* 
		char* ptr;
		ctArray.array[0] = strtol(temp, &ptr, 10);
		ctArray.array[1] = strtol(ptr, &ptr, 10);
		
		for(int i = 2; i<numberOfChara-1; i++){
			ctArray.array[i] = strtol(ptr, &ptr, 10);
		}
		ctArray.array[numberOfChara] = strtol(ptr, NULL, 10);
		*/
		
		token = strtok(temp, tok);
		
		int count = 0;
		while(token != NULL){
			//printf("token = %s and count = %d\n", token, count);
			ctArray.array[count] = strtol(token, NULL, 10); //convert into long and store
			token = strtok(NULL, tok);
			count++;
		}
		
		
		/* 
		for(int i = 0; i<numberOfChara; i++){
			printf("array |%d| = %ld\n",i,ctArray.array[i]);
		}
		*/
		
		
		//decrypt
		char PT[1000],print[1000];
		
		printf("\n--------------------------------------------------------------\n");
		printf("Decrypted message is \"");
		for(int i = 1; i<numberOfChara; i++){
			PT[i] = bin_mod(ctArray.array[i-1], key.d, key.n);
			print[i-1] = PT[i] + '\0';
			printf("%c", print[i-1]);
		}
		printf("\".\n");
		freeArray(&ctArray);
		
	}else{
		printf("Error reading Encrypted file \"%s\"", name);
		
	}

}

void decryptBin(){
	
	Keys key = readPrivKey();
	FILE* fp;
	
	int* arr;
	size_t size;
	
	char* name = getEncryptedFileName();
	if( fp = fopen(name, "rb")){
		arr = readBin(fp, arr, &size, name);
		decryptBytes(arr, size, key);
		writeToFile(arr, size, 'b');
		free(arr);
	}else{
		printf("Error reading Encrypted file \"%s\"", name);
		
	}
	
	Keys empty = {0};
	key =  empty;
	
}

long bin_mod(long b, long d,long n){
	
	if (n == 1) return 0;
	
	int r = 1;
	b %= n;
	while(d > 0){
		if (d % 2 == 1) r = (r * b) % n;
		d = d >> 1;
		b = (b * b) % n;
	}
	return r;
	
}

int* readBin(FILE* fp, int* arr, size_t* size, char* name){
	char* in;
	fseek(fp,0,SEEK_END);
	*size = ftell(fp);
	rewind(fp);
	in = (char*)malloc(*size);
	arr = (int*)malloc(*size);
	size_t ret_code = fread(in, sizeof *in, *size, fp); 
	
	if(ret_code == *size){
		printf("Array read successfully, contents: \n");
		for(int n = 0; n < *size; ++n){
			printf("%d ", in[n]);
			arr[n] = (int)in[n];
		}
		printf("\n");
	}
	if (feof(fp)){
		printf("Error reading %s: unexpected end of file\n",name);
	}
	else if (ferror(fp)) {
		printf("Error reading %s", name);
	}
	
	fclose(fp);
	free(in);
	return arr;
	
}

void decryptBytes(int* arr, int size, Keys key){
	
	for(int i=0; i<size; i++){
		arr[i] = bin_mod(arr[i],key.d,key.n);
	}
	
}

void writeToFile(int* C,int size, char type){
	
	FILE * fp;
	char name[50];
	
	printf("\nEnter output file name: ");
	scanf("%s",name);
	myFlush();
	
	if(type == 'b'){
		fp = fopen (name, "wb");
		
		for(int i = 0; i < size; i++){
			fprintf(fp,"%d;",C[i]);
		}
		
		fclose(fp);
	}else{
		fp = fopen (name, "w");
		
		for(int i = 0; i < size; i++){
			fprintf(fp,"%d;",C[i]);
		}
		
		fclose(fp);
	}
	
}