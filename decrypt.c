#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//define Key struct so that i don't have to keep creating
typedef struct{
	int n,e,d;
} Keys;

void myFlush();
void decrypt();
void decryptBin();
Keys readPubKey();
Keys readPrivKey();
char* readEncrypted();
long bin_mod(long b, long e,long m);

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

Keys readPubKey(){
	
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
		return key;
		
		
	}else{
		printf("\nError reading file \"%s\"\n", name);
		readPrivKey();
	}
}

char* readEncrypted(){
	
	//get encrypted file
	char name[100];
	printf("Name of encrypted file you would like to decrypt(e.g. abc.txt): ");
	scanf("%s",name);
	myFlush();
	/* size_t size; */
	FILE* fp;
	
	int c;
	int count = 0;
	char C[1000];
	static char arr[1000];
	
	if(fp = fopen(name,"r")){
		fscanf(fp, "%s", C);
		
		printf("%s\n",C);
		
		const char tok[2] = ";";
		char* token = strtok(C, tok);
		
		int i = 0;
		while(token != NULL){
			printf("%s\n\n", token);
			arr[i] = *token;
			token = strtok(NULL, tok);
			i++;
		}
		return arr;
	}else{
		printf("Error reading file \"%s\"\n",name);
		readEncrypted();
		
	}
}


void decrypt(){
	
	//get keys
	Keys key = readPrivKey();
	char* C = readEncrypted();
	char P[1000], P2[1000];
	
	//decrypt
	for(int i=0;i<30;i++){
		/* P[i] = M[i]-'\0'; */
		printf("this is C[%d] %d\t",i,C[i]);
		P[i] = bin_mod(C[i],key.d,key.n);
		
		P2[i] = P[i]+'\0';
		printf("this is P %d\n",P2[i]);
	}

}

void decryptBin(){
	printf("test2");
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