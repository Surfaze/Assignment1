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

void seedRandom(){
	time_t t;
	srand((unsigned)time(&t));
}
long primeNumberAbove(long in){
	in++;
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
	int i = 2;
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
		p = randomInt(1000,2000);
		q = randomInt(1000,2000);
		printf("%d\t%d\n",p,q);
	}while(isPrime(p)!=0 || isPrime(q)!=0);
	
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
		//Retarded inefficient method that doesnt work -.-
		//C[i] = pow(P[i],key.e) % key.n;
		int k;
		int step =1;
		
		printf("\ntest\t%d\t",step);
		
		for(k = 0;k<key.e;k++){
			step = (step * P[i]) % key.n;
			printf("%d\t",step); 
		}
		printf("end\t");
		
		C[i] = step;
		printf("%d\n",C[i]);
	}
	printf("\n\nEncrypted\n---------------------\n");
	
	for(x = 0; x< size;x++){
		printf("%d\t",C[x]);
	}
	//writing to output file
	writeToFile(C,size);
}


void main(void){
	encrypt();
}