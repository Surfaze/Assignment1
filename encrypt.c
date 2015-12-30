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

void main(void){
	
	int choice;
	do{
		printf("Action:\n1. Encrypt Message\n2. Encrypt Binary File\n3. Create a key pair\n4. Exit\nYour choice: ");
		scanf("%d", &choice);
		myFlush();
		printf("\n--------------------------------------------------------------\n");
	}while(choice < 1 || choice > 4);
	(choice == 1)? encrypt() : (choice == 2) ? encryptBin() :  (choice == 3) ? genKey() : (choice == 4) ? exit(0) : 0;
	
}

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
	int P[1000], C[1000];
	Keys key = readPubKey();

	
	printf("Enter Message: ");
	gets(M);
	printf("\n\nPlain Text\n-----------------\n");
	size = strlen(M);
	
	
	for(i=0;i<size;i++){
		P[i] = M[i]-'\0';
		printf("%d\t",P[i]);
		C[i] = bin_mod(P[i],key.e,key.n);
		printf("%d\n",C[i]);
	}
	printf("\n\nEncrypted\n---------------------\n");
	
	for(x = 0; x< size;x++){
		printf("%d\t",C[x]);
	}
	
	writeToFile(C,size,'f');
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