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
	
	seedRandom();
	long p,q;
	do{
		p = randomInt(2,100);
		q = randomInt(2,100);
		printf("%d\t%d\n",p,q);
	}while(isPrime(p)!=0 || isPrime(q)!=0);
	
	printf("---------------\n");
	long n = p*q;
	long tot = (p-1)*(q-1);
	
	int i;
	long d;
	long e;
	
	int j;
	
	for(j=0; j <10; j++){
		i++;
		d = tot*i+1;
		e = d;
		int div=2;
		
		do{
			if(e%div==0){
				e /= div;
			}else{
				div++;
			}
		}while(isPrime(e)==1);
		
		if(!(e == d)){
			//printf("%d\t%d\n",e, d/e);
		}else j--;
	}
	
	printf("\np: %ld\nq: %ld\nn: %ld\ntot: %ld\n",p,q,n,tot);
	
	Keys key;
	key.n = n;
	key.d=d/e;
	key.e=e;
	
	printf("\nn: %ld\nd: %ld\ne: %ld\n",key.n,key.d,key.e);
	
	char M[100];
	
	printf("Enter Message: ");
	gets(M);
	
	printf("\n\nPlain Text\n-----------------\n");
	int size = strlen(M);
	int P[100],C[100];
	
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
	int x;
	for(x = 0; x< size;x++){
		printf("%d\t",C[x]);
	}
	writeToFile(C,size);
}


void main(void){
	encrypt();
}