#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

void myFlush();
void seedRandom();
long randomInt(long min, long max);
int isPrime(long x);
void writeToFile(int* C,int size);
long bin_mod(long b, long e,long m);
void encrypt();
void readBinBytes(char* name);

typedef struct{
	int n;
	int e;
	int d;
} Keys;

void main(void){
	int choice;
	do{
		printf("Action:\n1. Encrypt Message\n2. Encrypt Binary File\n3. Exit\nYour choice: ");
		scanf("%d", &choice);
		myFlush();
		printf("\n--------------------------------------------------------------\n");
	}while(choice < 1 || choice > 3);
	(choice == 1)? encrypt() : (choice == 2) ? readBinBytes("test.bin") :  (choice == 3) ? exit(0) : 0;
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
		if (in % 2 == 0){
			continue;
		}
		if(isPrime(in)==0){
			return in;
		}
	}
}

long randomInt(long min, long max){
	return rand()%(max-min) + min;
}

int isPrime(long x){
	if (x % 2 == 0){
		return 1;
	}
	long max = floor(sqrt(x));
	int i = 1;
	for (; i < max; i++) {
		if (x % ((2 * i) + 1) == 0){
			return 1;
		}
	}
	return 0;
}
void writeToFile(int* C,int size){
	FILE * fp;
	fp = fopen ("cipher.txt", "w+");
	
	int i;
	for(i = 0; i < size; i++){
		fprintf(fp,"%d;",C[i]);
	}
	
	fclose(fp);
}
	
long bin_mod(long b, long e,long m){
	if (m == 1) return 0;
	int r =1;
	b %= m;
	while(e > 0){
		if (e % 2 == 1) r = (r * b) % m;
		e = e >> 1;
		b = (b*b) % m;
	}
	return r;
}

void encrypt(){
	//Declarations
	long long p, q, d, e, n, tot;
	int i, j, size, x;
	char M[100];
	int P[100], C[100];
	//Seed random
	seedRandom();
	//Generate 2 random primes p & q
	do{
		//cannot do more 100k, overflow maybe?
		p = prime(randomInt(10000,20000));
		q = prime(randomInt(10000,20000));
		printf("%lld\t%lld\n",p,q);
	}while(isPrime(p)!=0 && isPrime(q)!=0);
	
	printf("---------------\n");
	//Calculating n and totient
	n = p*q;
	tot = (p-1)*(q-1);

	//Solving for d & e
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
	//Storing values into Keys struct
	Keys key;
	key.n = n;
	key.d=d/e;
	key.e=e;
	
	printf("\nn: %lld\nd: %lld\ne: %lld\n",key.n,key.d,key.e);

	//Retriving user message to encrypt
	printf("Enter Message: ");
	gets(M);
	printf("\n\nPlain Text\n-----------------\n");
	//Calculating size of message
	size = strlen(M);
	
	//Encrypting message
	for(i=0;i<size;i++){
		P[i] = M[i]-'\0';
		printf("%lld\t",P[i]);
		C[i] = bin_mod(P[i],key.e,key.n);
		printf("%lld\n",C[i]);
	}
	printf("\n\nEncrypted\n---------------------\n");
	
	for(x = 0; x< size;x++){
		printf("%lld\t",C[x]);
	}
	//writing to output file
	//writeToFile(C,size);
	printf("\n\nDecrypted\n---------------------\n");
	for(i=0;i<size;i++){
		P[i] = C[i];
		printf("%lld\t",P[i]);
		C[i] = bin_mod(P[i],key.d,key.n);
		printf("%lld\n",C[i]);
	}
}

void readBinBytes(char* name){
	
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
   }
   else if (ferror(fp)) {
		printf("Error reading %s", name);
   }
}
