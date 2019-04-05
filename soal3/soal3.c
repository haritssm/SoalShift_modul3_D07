#include<stdio.h>
#include<string.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>

pthread_t tid[4]; //inisialisasi array untuk menampung thread dalam kasus ini ada 2 thread
//int counter = 0;
//int length=5; //inisialisasi jumlah untuk looping
char str[50];
int WakeUp_Status,Spirit_Status,x;
int Agmal,Iraj,Ag,Ir;
/*struct arg{
	char str[1000];
	int counter;
};*/

void* A(void *args){
	while(1){
		if(x==1) break;
		if(Ag==1){
			//if(x==1) break;
			//if(Iraj==3){
			//	printf("Agmal Ayo Bangun disabled 10 s\n");
			//	sleep(10);
			//	Iraj=0;
			//}
			WakeUp_Status+=15;
			Agmal++;
			Ag=0;
		}
		if(Iraj==3){
			printf("Agmal Ayo Bangun disabled 10 s\n");
			sleep(10);
			Iraj=0;
		}
		sleep(1);
	}
}

void* I(void *args){
	while(1){
		if(x==1) break;
		if(Ir==1){
			//if(x==1) break;
			//if(Agmal==3){
			//	printf("Iraj Ayo Tidur disabled 10s\n");
			//	sleep(10);
			//	Agmal=0;
			//}
			Spirit_Status-=20;
			Iraj++;
			Ir=0;
		}
		if(Agmal==3){
			printf("Iraj Ayo Tidur disavled 10s\n");
			sleep(10);
			Agmal=0;
		}
		sleep(1);
	}
}

void* in(void *args){
	char str[50];
	while(1){
		//if(x==1) break;
		scanf("%s",str);
		if(strcmp(str,"exit")==0){
			x=1;
			break;
		}
		if(strcmp(str,"All_Status")==0) printf("Agmal WakeUp_Status= %d\nIraj Spirit_Status= %d\n",WakeUp_Status,Spirit_Status);
		if(strcmp(str,"Agmal_Ayo_Bangun")==0){
			Ag=1;
			/*if(Agmal==3){
				sleep(10);
				Agmal=0;
			}
			WakeUp_Status+=15;
			Agmal++;*/
		}
		if(strcmp(str,"Iraj_Ayo_Tidur")==0){
			Ir=1;
			/*if(Iraj==3){
				sleep(10);
				Iraj=0;
			}
			Spirit_Status-=20;
			Iraj++;*/
		}
		sleep(1);
	}
}

void* fitur(void *args){//buat masing2 fitur thread sendiri2
	while(1){
		if(x==1) break;
		if(WakeUp_Status>99){
			printf("Agmal Terbangun, mereka bangun pagi dan berolahraga\n");
			x=1;
			break;
		}
		if(Spirit_Status<1){
			printf("Iraj ikut tidur, dan bangun kesiangan bersama Agmal\n");
			x=1;
			break;
		}
		//sleep(1);
	}
}


int main()
{
	//struct arg *arg1,*arg2,args1,args2;
	//arg1 = &args1;
	//arg2 = &args2;

	//args1.str = "";
	//args2.str = "";

	int i=0;
	int err1,err2,err3,err4;
 	FILE *f;
	//char *arg;
	//char str[100];
	WakeUp_Status = 0;
	Spirit_Status = 100;
	x = 0;
	Agmal=0;
	Iraj=0;
	Ag=0;
	Ir=0;
	//args1.str[0]="";
	//args2.str[0]="";
	//strcpy(args1.str,"");
	//strcpy(args2.str,"");

	//arg1->str = malloc(sizeof(char) * 800);
	//arg2->str = malloc(sizeof(char) * 800);
	/*f = popen("ps aux | tail -n 10","r");
	while(fgets(str,sizeof(str),f) != NULL){
		strcat(args1.str,str);
		strcat(args2.str,str);
	}
	pclose(f);
	args1.counter = 1;
	args2.counter = 2;

	printf("%d\n%s\n",args1.counter,args1.str);
	printf("%d\n%s\n",args2.counter,args2.str);
	*/
	printf("Program Start...\n");
	err1 = pthread_create(&(tid[0]),NULL,&in,NULL);
	if(err1) exit(EXIT_FAILURE);
	err2 = pthread_create(&(tid[1]),NULL,&fitur,NULL);
	if(err2) exit(EXIT_FAILURE);
	err3 = pthread_create(&(tid[2]),NULL,&A,NULL);
	if(err3) exit(EXIT_FAILURE);
	err4 = pthread_create(&(tid[3]),NULL,&I,NULL);
	if(err4) exit(EXIT_FAILURE);

	//printf("create thread 1 return: %d\n",err1);
	//printf("create thread 2 return: %d\n",err2);

	pthread_join(tid[0],NULL);
	pthread_join(tid[1],NULL);
	pthread_join(tid[2],NULL);
	pthread_join(tid[3],NULL);
	/*while(i<2) // loop sejumlah thread
	{
		err=pthread_create(&(tid[i]),NULL,&proses,(void*) arg); //membuat thread
		if(err!=0) //cek error
		{
			printf("\n can't create thread : [%s]",strerror(err));
		}
		else
		{
			printf("\n create thread success\n");
		}
		i++;
		//counter++;
	}

	pthread_join(tid[1],NULL);
	pthread_join(tid[2],NULL);
	exit(0);*/
	return 0;
}
