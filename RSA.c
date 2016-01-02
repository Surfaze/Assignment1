#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define KEYMIN 0
#define KEYMAX 50000

typedef struct{
	int n;
	int e;
	int d;
} Keys;

typedef struct{
	long *array;
	size_t size;
} Array;

//Function headers
void welcomeMsg();
void myFlush();
void seedRandom();
long prime(long in);
long randomInt(long min, long max);
int isPrime(long x);
void writeToFile(int* C,int size, char type);
long bin_mod(long b, long e,long m);
void genKey();
void writeKeys(Keys key);
Keys readPubKey();
int* readBin(FILE* fp, int* arr, size_t* size, char* name);
void encryptBytes(int* arr, int size, Keys key);
void encrypt();
void encryptBin();
void decrypt();
void decryptBin();
Keys readPrivKey();
Keys readPrivKey2();
char* getEncryptedFileName();
char* readEncrypted();
void initArray(Array *a, size_t initialSize);
void freeArray(Array *a);
void decryptBytes(int* arr, int size, Keys key);

//Main
int main() {
	
	welcomeMsg();
	return 0;
	
}

void welcomeMsg() {
	
	int choice;
	printf("*****************************************\n");
	printf("*\t%-25s\t*\n","ST2614 Assignment 1 done by:","*");
	printf("*\t%-25s\t*\n","Chong Jia Hao P1430615");
	printf("*\t%-25s\t*\n","Nicholas Koh P1431207");
	printf("*\t%-25s\t*\n","Class 2A / 21");
	printf("*****************************************\n\n");

	do{
		printf("Action:\n1. Encrypt Message\n2. Encrypt Binary File\n3. Create a key pair\n4. Decrypt Message\n5. Decrypt Binary File\n6. Exit\nYour choice: ");
		scanf("%d", &choice);
		myFlush();
		printf("\n--------------------------------------------------------------\n");
	}while(choice < 1 || choice > 4);
	(choice == 1)? encrypt() : (choice == 2) ? encryptBin() :  (choice == 3) ? genKey() : (choice == 4) ? decrypt() : (choice == 5) ? decryptBin() : (choice == 6 ) ? printf("Goody Bye.") : 0;

}


//===========================Encryption Function by Nicholas====================================
void myFlush(){
	
	while(getchar()!='\n');
	
}

void seedRandom(){
	
	time_t t;
	srand((unsigned)time(&t));
	
}

long prime(long in){
	
	for(;;in++){
		if(in % 2 == 0)continue;
		if(isPrime(in)==0)return in;
	}
	
}

long randomInt(long min, long max){
	
	return rand()%(max-min) + min;
	
}

int isPrime(long x){
	
	if (x % 2 == 0)return 1;
	
	long max = floor(sqrt(x));
	for (int i = 1; i < max; i++) {
		if (x % ((2 * i) + 1) == 0) return 1;
	}
	return 0;
	
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
	
long bin_mod(long b, long e,long m){
	//using binary modular exponentiation to further improve efficiency as modular exponentiation is slower than this
	
	if (m == 1) return 0;
	
	int r = 1;
	b %= m;
	while(e > 0){
		if (e % 2 == 1) r = (r * b) % m;
		e = e >> 1;
		b = (b * b) % m;
	}
	return r;
	
}

void genKey(){
	
	int p,q,i,j;
	long long n,tot,d,e;
	
	seedRandom();
	
	do{
		//value larger = slower, by ALOT, cause of multiplication
		p = prime(randomInt(KEYMIN,KEYMAX));
		q = prime(randomInt(KEYMIN,KEYMAX));
		printf("%lld\t%lld\n",p,q);
	}while(isPrime(p)!=0 && isPrime(q)!=0);
	
	printf("---------------\n");
	
	n = p*q;
	tot = (p-1)*(q-1);

	
	for(j=0; j <10; j++){
		i++;
		d = tot*i+1;
		e = d;
		
		int div=2;
		
		do{
			(e%div==0)? e /= div : div++;
			
		}while(isPrime(e)==1);
		
		if((e == d)) j--;
	}
	
	printf("\np: %lld\nq: %lld\nn: %lld\ntot: %lld\n",p,q,n,tot);
	
	Keys key;
	key.n = n;
	key.d=d/e;
	key.e=e;
	
	printf("\nn: %lld\nd: %lld\ne: %lld\n\n",key.n,key.d,key.e);

	writeKeys(key);
	
}

void writeKeys(Keys key){
	
	FILE * fp;
	char out[50],out1[50];
	
	do{
		printf("Enter your name: ");
		scanf("%s",out);
		myFlush();
		printf("\nPlease re-enter your name to confirm: ");
		scanf("%s",out1);
		myFlush();
	}while(strcmp(out,out1) != 0? printf("\nNames do not match.\n"):0);

	char* priv = strcat(out,"_priv.txt");
	char* pub = strcat(out1,"_pub.txt");
	
	fp = fopen (priv, "w+");
	fprintf(fp,"%d;%d",key.n,key.d);
	fclose(fp);
	
	fp = fopen (pub, "w+");
	fprintf(fp,"%d;%d",key.n,key.e);
	fclose(fp);

}

Keys readPubKey(){
	
	FILE* fp;
	Keys key;
	char name[100], in[100];
	size_t size;
		
	printf("Name of public key file you would like to use(e.g. xxx_pub.txt): ");
	scanf("%s",name);
	myFlush();
	
	if(fp = fopen(name, "r")){
		fseek(fp,0,SEEK_END);
		size = ftell(fp);
		rewind(fp);
		size_t ret_code = fread(in, 1, size, fp);
		if(ret_code==size){
			const char tok[2] = ";";
			char* token = strtok(in, tok);
			key.n = atoi(token);
			token = strtok(NULL,tok);
			key.e = atoi(token);
			return key;
		}else{
			printf("\nError reading file %s\n", name);
			readPubKey();
		}
	}else{
		printf("\nError reading file %s\n", name);
		readPubKey();
	}
	
}

Keys readPrivKey(){
	
	FILE* fp;
	Keys key;
	char name[100], in[100];
	size_t size;
		
	printf("Name of private key file you would like to use(e.g. xxx_priv.txt): ");
	scanf("%s",name);
	myFlush();
	
	if(fp = fopen(name, "r")){
		fseek(fp,0,SEEK_END);
		size = ftell(fp);
		rewind(fp);
		size_t ret_code = fread(in, 1, size, fp);
		if(ret_code==size){
			const char tok[2] = ";";
			char* token = strtok(in, tok);
			key.n = atoi(token);
			token = strtok(NULL,tok);
			key.d = atoi(token);
			return key;
		}else{
			printf("\nError reading file %s\n", name);
			readPubKey();
		}
	}else{
		printf("\nError reading file %s\n", name);
		readPubKey();
	}
	
}

void encryptBytes(int* arr, int size, Keys key){
	
	for(int i=0;i<size;i++){
		arr[i] = bin_mod(arr[i],key.e,key.n);
	}
	
}

void encrypt(){

	int size, x, i;
	char M[1000];
	int P[1000];
	Keys key = readPubKey();

	
	printf("Enter Message: ");
	gets(M);
	printf("\n\nPlain Text\n-----------------\n");
	size = strlen(M);
	
	
	for(i=0;i<size;i++){
		P[i] = M[i]-'\0';
		printf("%d\t",P[i]);
		P[i] = bin_mod(P[i],key.e,key.n);
		printf("%d\n",P[i]);
	}
	printf("\n\nEncrypted\n---------------------\n");
	
	for(x = 0; x< size;x++){
		printf("%d\t",P[x]);
	}
	
	writeToFile(P,size,'f');
	Keys empty = {0};
	key = empty;
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

void encryptBin(){
	
	Keys key = readPubKey();
	FILE* fp;

	int* arr;
	size_t size;
	
	printf("File name to encrypt: ");
	char* name;
	scanf("%s",name);
	myFlush();
	

	if(fp = fopen(name,"rb")){
		
		arr = readBin(fp,arr,&size,name);
		encryptBytes(arr, size, key);
		
		writeToFile(arr, size,'b');
		free(arr);
	}else{
		printf("Error reading %s", name);
	}
	
	Keys empty = {0};
	key = empty;
	
}
//=============================End Encryption Function======================================



//===========================Decryption Function by Jia Hao====================================
Keys readPrivKey2(){
	
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
		readPrivKey2();
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
	Keys key = readPrivKey2();
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
	
	Keys key = readPrivKey2();
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

void decryptBytes(int* arr, int size, Keys key){
	
	for(int i=0; i<size; i++){
		arr[i] = bin_mod(arr[i],key.d,key.n);
	}
	
}
//=============================End Decryption Function======================================