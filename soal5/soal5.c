#include<stdio.h>
#include<string.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<sys/ipc.h>
#include<sys/shm.h>

pthread_t tid[4]; //inisialisasi array untuk menampung thread dalam kasus ini ada 2 thread
//int counter = 0;
//int length=5; //inisialisasi jumlah untuk looping
//char str[50];
//int WakeUp_Status,Spirit_Status,x;
//int Agmal,Iraj,Ag,Ir;

int shmid,*stock,x;
char str[100];

struct monster{
	char nama[1000];
	int health;
	int hunger;
	int hygiene;
	int stok;
	int stat;
};

int aksi,kondisi,time;

void* scan(void *args){
	while(1){
		scanf("%s",str);
		if(strcmp(str,"Makan")==0) aksi = 1;
		if(strcmp(str,"Mandi")==0) aksi = 2;
		if(strcmp(str,"Battle")==0) kondisi = 1;
		if(strcmp(str,"Shop")==0) kondisi = 2;
		sleep(1);
	}
}

void* standby(void *args){
	while(1){
		if(kondisi==0){
			/*if(aksi==0){
				
			}
			if(aksi==1){//makan
				if(utama.stok>0){
					utama.hunger += 15;
					utama.stok--;
				}
			}
			if(aksi==2){//mandi
				if(utama.stat==1){
					
				}
			}*/
			printf("Standby Mode\n");
			printf("Monster: %s\n",utama.nama);
			printf("Health: %d\n",utama.health);
			printf("Hunger: %d\n",utama.hunger);
			printf("Hygiene; %d\n",utama.hygiene);
			printf("Food left; %d\n",utama.stok);
			if(utama.stat==1) printf("Bath ready\n");
			else printf("Bath will be ready in %d s\n",time);
		}
		sleep(1);
	}
}

void* battle(void *args){
	while(1){

	}
}

void* shop(void * args){
	while(1){

	}
}

int main()
{
	int err1,err2,err3,err4;
	key_t key = 1111;
	shmid = shmget(key,sizeof(int),IPC_CREAT | 0666);
	stock = shmat(shmid,NULL,0);

	printf("Program Start...\n");

	struct monster utama;
	printf("Masukan Nama Monster Anda:")
	scanf("%s",utama.nama);
	time = 0;
	aksi = 0;
	kondisi = 0;
	utama.health = 300;
	utama.hunger = 200;
	utama.hygiene = 100;
	utama.stok = 0;
	utama.stat = 1;

	printf("Selamat bermain :)\n");
	system("clear");

	err1 = pthread_create(&(tid[0]),NULL,&scan,NULL);
	if(err1) exit(EXIT_FAILURE);
	err2 = pthread_create(&(tid[1]),NULL,&standby,NULL);
	if(err2) exit(EXIT_FAILURE);
	err3 = pthread_create(&(tid[2]),NULL,&battle,NULL);
	if(err3) exit(EXIT_FAILURE);
	err4 = pthread_create(&(tid[3]),NULL,&shop,NULL);
	if(err4) exit(EXIT_FAILURE);


	pthread_join(tid[0],NULL);
	pthread_join(tid[1],NULL);
	pthread_join(tid[2],NULL);
	pthread_join(tid[3],NULL);

	shmdt(stock);
	shmctl(shmid,IPC_RMID,NULL);
	return 0;
}
