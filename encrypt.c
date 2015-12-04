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
void encrypt(){
	seedRandom();
	long p = primeNumberAbove(randomInt(1000,2000));
	long q = primeNumberAbove(p);
	
	long n = p*q;
	long tot = (p-1)*(q-1);
	
	int i;
	long d[10];
	printf("Possible values of d & e\n");
	
	do{
		i++;
		d[i-1] = tot*i+1;
		long e = d[i-1];
		int div=2;
		
		do{
			if(e%div==0){
				e /= div;
			}else{
				div++;
			}
		}while(isPrime(e)==1);
		
		if(!(e == d[i-1])){
			printf("%d\t%d\n",e, d[i-1]/e);
		}
	}while(i<=10);
	
	
	
	printf("\np: %d\nq: %d\nn: %d\ntot: %d\n",p,q,n,tot);
}


void main(void){
	encrypt();
}