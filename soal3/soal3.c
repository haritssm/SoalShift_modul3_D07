#include<stdio.h>
#include<string.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>

pthread_t tid[4]; //inisialisasi array untuk menampung thread dalam kasus ini ada 2 thread
int WakeUp_Status,Spirit_Status;
int Agmal,Iraj,Ag,Ir;

void* A(void *args){
	while(1){
		//if(x==1) break;
		if(Iraj==3){
			printf("Agmal Ayo Bangun disabled 10 s\n");
			Ag=0;
			sleep(10);
			Iraj=0;//revisi
		}
		if(Ag==1){
			WakeUp_Status+=15;
			Agmal++;
			Ag=0;
		}
		sleep(1);
	}
}

void* I(void *args){
	while(1){
		//if(x==1) break;
		if(Agmal==3){
			printf("Iraj Ayo Tidur disabled 10 s\n");
			Ir=0;
			sleep(10);
			Agmal=0;//revisi
		}
		if(Ir==1){
			Spirit_Status-=20;
			Iraj++;
			Ir=0;
		}
		sleep(1);
	}
}

void* in(void *args){
	char str[50];
	while(1){
		scanf("%s",str);
		if(strcmp(str,"exit")==0){
			//x=1;
			//break;
			exit(0);
		}
		if(strcmp(str,"All_Status")==0) printf("Agmal WakeUp_Status= %d\nIraj Spirit_Status= %d\n",WakeUp_Status,Spirit_Status);
		if(strcmp(str,"agmal_ayo_bangun")==0){
			Ag=1;
		}
		if(strcmp(str,"iraj_ayo_tidur")==0){
			Ir=1;
		}
		sleep(1);
	}
}

void* fitur(void *args){//buat masing2 fitur thread sendiri2
	while(1){
		//if(x==1) break;
		if(WakeUp_Status>99){
			printf("Agmal Terbangun, mereka bangun pagi dan berolahraga\n");
			//x=1;
			//break;
			exit(0);
		}
		if(Spirit_Status<1){
			printf("Iraj ikut tidur, dan bangun kesiangan bersama Agmal\n");
			//x=1;
			//break;
			exit(0);
		}
		//sleep(1);
	}
}


int main()
{
	int i=0;
	int err1,err2,err3,err4;
	WakeUp_Status = 0;
	Spirit_Status = 100;
	//x = 0;
	Agmal=0;
	Iraj=0;
	Ag=0;
	Ir=0;

	printf("Program Start...\n");
	err1 = pthread_create(&(tid[0]),NULL,&in,NULL);
	if(err1) exit(EXIT_FAILURE);
	err2 = pthread_create(&(tid[1]),NULL,&fitur,NULL);
	if(err2) exit(EXIT_FAILURE);
	err3 = pthread_create(&(tid[2]),NULL,&A,NULL);
	if(err3) exit(EXIT_FAILURE);
	err4 = pthread_create(&(tid[3]),NULL,&I,NULL);
	if(err4) exit(EXIT_FAILURE);

	pthread_join(tid[0],NULL);
	pthread_join(tid[1],NULL);
	pthread_join(tid[2],NULL);
	pthread_join(tid[3],NULL);

	return 0;
}
