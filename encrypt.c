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
	long p,q;
	do{
		p = randomInt(1000,2000);
		q = randomInt(1000,2000);
	}while(isPrime(p)!=0 && isPrime(q)!=0);
	
	long n = p*q;
	long tot = (p-1)*(q-1);
	
	int i;
	//long d[10];
	long d;
	//printf("Possible values of d & e\n");
	long e;
	
	
	
	do{
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
			printf("%d\t%d\n",e, d/e);
		}
	}while(i<=10);
	
	printf("\np: %ld\nq: %ld\nn: %ld\ntot: %ld\n",p,q,n,tot);
	
	Keys key;
	key.n = n;
	key.d=d/e;
	key.e=e;
	
	printf("\nn: %ld\nd: %ld\ne: %ld\n",key.n,key.d,key.e);
}


void main(void){
	encrypt();
}