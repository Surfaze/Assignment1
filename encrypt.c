#include "util.h"
#include <math.h>
#include <stdio.h>

typedef struct{
	int n;
	int e;
	int d;
} Keys;

void encrypt(){
	seedRandom();
	int p = primeNumberAbove(randomInt(0,100));
	int q = primeNumberAbove(p);
	
	int n = p*q;
	int tot = (p-1)*(q-1);
	
	int i;
	int d[10];
	printf("Possible values of d & e\n");
	
	do{
		i++;
		d[i-1] = tot*i+1;
		int e = d[i-1],div=2;
		
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