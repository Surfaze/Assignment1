#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

void seedRandom();
long primeNumberAbove(long in);
long randomInt(long min, long max);
int isPrime(long x);

typedef struct{
	int n;
	int e;
	int d;
} Keys;

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
	long p, q, d, e, n, tot;
	int i, j, size, x;
	char M[100];
	int P[100], C[100];
	//Seed random
	seedRandom();
	//Generate 2 random primes p & q
	do{
		p = prime(randomInt(1000,2000));
		q = prime(randomInt(1000,2000));
		printf("%d\t%d\n",p,q);
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
	
	printf("\np: %ld\nq: %ld\nn: %ld\ntot: %ld\n",p,q,n,tot);
	//Storing values into Keys struct
	Keys key;
	key.n = n;
	key.d=d/e;
	key.e=e;
	
	printf("\nn: %ld\nd: %ld\ne: %ld\n",key.n,key.d,key.e);

	//Retriving user message to encrypt
	printf("Enter Message: ");
	gets(M);
	printf("\n\nPlain Text\n-----------------\n");
	//Calculating size of message
	size = strlen(M);
	
	//Encrypting message
	for(i=0;i<size;i++){
		P[i] = M[i]-'\0';
		printf("%d\t",P[i]);
		
		//------------------modular method start------------------
		/*int k;
		int step =1;
		
		printf("\ntest\t%d\t",step);
		
		for(k = 0;k<key.e;k++){
			step = (step * P[i]) % key.n;
			printf("%d\t",step); 
		}
		printf("end\t");
		
		C[i] = step;*/
		//------------------modular method end------------------
		
		//------------------binary method start------------------
		C[i] = bin_mod(P[i],key.e,key.n);
		//------------------binary method end------------------
		printf("%d\n",C[i]);
	}
	printf("\n\nEncrypted\n---------------------\n");
	
	for(x = 0; x< size;x++){
		printf("%d\t",C[x]);
	}
	//writing to output file
	writeToFile(C,size);
}

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

void main(void){
	int choice;
	do{
		printf("Action:\n1. Encrypt Message\n2. Encrypt Binary File\n3. Exit\nYour choice: ");
		scanf("%d", &choice);
		myFlush();
		printf("\n--------------------------------------------------------------\n");
	}while(choice < 1 || choice > 3);
	(choice == 1)? encrypt() : (choice == 2) ? encryptFile("test.bin") :  (choice == 3) ? exit(0) : 0;
}