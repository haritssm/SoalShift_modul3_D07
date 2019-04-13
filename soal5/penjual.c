#include<stdio.h>
#include<string.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<sys/ipc.h>
#include<sys/shm.h>

pthread_t tid[2];
int shmid,*stock;
int aksi;

void* scan(void *args){
	int anjay;
	while(1){
		scanf("%d",&anjay);
		if(anjay==1) aksi=1;
		if(anjay==2){
			aksi=2;
			break;
		}

		sleep(1);
	}
}

void* standby(void *args){
	while(1){
		if(aksi==1){//makan
			(*stock)++;
			aksi=0;
		}
		if(aksi==2){//exit
			break;
		}
		printf("Shop\n");
		printf("Food stock : %d\n",*stock);
		printf("Choices\n");
		printf("1. Restock\n");
		printf("2. Exit\n");

		sleep(1);
		system("clear");
	}
}


int main()
{
	int err1,err2;
	key_t key = 1111;
	shmid = shmget(key,sizeof(int),IPC_CREAT | 0666);
	stock = shmat(shmid,NULL,0);

	err1 = pthread_create(&(tid[0]),NULL,&scan,NULL);
	if(err1) exit(EXIT_FAILURE);
	err2 = pthread_create(&(tid[1]),NULL,&standby,NULL);
	if(err2) exit(EXIT_FAILURE);


	pthread_join(tid[0],NULL);
	pthread_join(tid[1],NULL);
	pthread_join(tid[2],NULL);
	//pthread_join(tid[3],NULL);

	shmdt(stock);
	shmctl(shmid,IPC_RMID,NULL);
	return 0;
}
