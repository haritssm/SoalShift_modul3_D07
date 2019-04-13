#include<stdio.h>
#include<string.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<termios.h>

//ini bagian revisi input
static struct termios old, new;

/*initialize new terminal i/o settings */
void initTermios(int echo)
{
  	tcgetattr(0, &old); /* grab old terminal i/o settings */
  	new = old; /* make new settings same as old settings */
  	new.c_lflag &= ~ICANON; /* disable buffered i/o */
  	if (echo) {
      		new.c_lflag |= ECHO; /* set echo mode */
  	} else {
      		new.c_lflag &= ~ECHO; /* set no echo mode */
  	}
  	tcsetattr(0, TCSANOW, &new); /* use these new terminal i/o settings now */
}

/* Restore old terminal i/o settings */
void resetTermios(void)
{
  	tcsetattr(0, TCSANOW, &old);
}

/* Read 1 character - echo defines echo mode */
char getch_(int echo)
{
  	char ch;
  	initTermios(echo);
  	ch = getchar();
  	resetTermios();
  	return ch;
}

/* Read 1 character without echo */
char getch(void) 
{
  	return getch_(0);
}

pthread_t tid[4];
int shmid,*stock,x;
//char str[100];

struct monster{
	char nama[100];
	int health;
	int hunger;
	int hygiene;
	int stok;
	int stat;
};

struct monster utama;
int aksi,kondisi,t,globalTime,status;

void battle(){
	struct monster npc;
	npc.health = 100;
	int counter=0;
	system("clear");
	while(1){
		if(kondisi==1){
			if(npc.health<1){
				utama.stok+=2;//tambahan jika menang
				break;
				system("clear");
			}
			if(aksi==3){
				npc.health-=20;
				counter=1;
				aksi=0;
				system("clear");
			}
			if(counter==1){
				system("clear");
				utama.health-=20;
				printf("Battle Mode\n");
				printf("Monster's Health : %d\n",utama.health);
				printf("Enemy's Health : %d\n",npc.health);
				printf("Enemy attack you\n");
				sleep(1);
				system("clear");
				counter=0;
			}

			printf("Battle Mode\n");
			printf("Moster's Health : %d\n",utama.health);
			printf("Enemy's Health : %d\n",npc.health);
			printf("Choices\n");
			printf("1. Attack\n");
			printf("2. Run\n");
		}
		if(kondisi==0) break;

		sleep(1);
		system("clear");
	}
	status=0;
}

void shop(){
	while(1){
		if(kondisi==2){
			if(aksi==4){
				utama.stok++;
				(*stock)--;
				aksi=0;
				system("clear");
			}
			printf("Shop Mode\n");
			printf("Shop food stock : %d\n",*stock);
			printf("Your food stock : %d\n",utama.stok);
			printf("Choices\n");
			printf("1. Buy\n");
			printf("2. Back\n");
		}
		if(kondisi==0) break;

		sleep(1);
		system("clear");
	}
	status=0;
}

void* scan(void *args){
	char anjay;
	while(1){
		/*scanf("%s",str);
		if(kondisi==0){
			if(strcmp(str,"Makan")==0) aksi = 1;//makan
			if(strcmp(str,"Mandi")==0) aksi = 2;//mandi
			if(strcmp(str,"Battle")==0) kondisi = 1;
			if(strcmp(str,"Shop")==0) kondisi = 2;
		}
		if(kondisi==1){
			if(strcmp(str,"Attack")==0) aksi = 3;//attack
			if(strcmp(str,"Run")==0) kondisi = 0;
		}
		if(kondisi==2){
			if(strcmp(str,"Buy")==0) aksi = 4;//buy
			if(strcmp(str,"Back")==0) kondisi = 0;
		}
		if(strcmp(str,"Exit")==0){
			x=1;
			break;
		}*/
		//scanf("%d",&anjay);
		//if(kbhit()){
		anjay = getch();
		if(kondisi==0){
			if(anjay=='1') aksi=1;//makan
			if(anjay=='2') aksi=2;//mandi
			if(anjay=='3'){
				kondisi=1;//battle
				status=1;
			}
			if(anjay=='4'){
				kondisi=2;//shop
				status=2;
			}
			if(anjay=='5') {
				x=1;
				break;
			}
		}
		if(kondisi==1){
			if(anjay=='1') aksi=3;
			if(anjay=='2') kondisi=0;
		}
		if(kondisi==2){
			if(anjay=='1') aksi=4;
			if(anjay=='2') kondisi=0;
		}
		sleep(1);
	}
}

void* standby(void *args){
	while(1){
		if(x==1) break;
		if(kondisi==0){
			if(globalTime%10==0){
				utama.hunger-=5;
				utama.health-=5;
				system("clear");
			}
			if(utama.health<1) break;
			if(utama.hunger<1) break;

			if(globalTime%30==0) {
				utama.hygiene-=10;
				system("clear");
			}
			if(utama.hygiene<1) break;

			if(globalTime)

			if(aksi==1){//makan
				if(utama.stok>0){
					utama.hunger += 15;
					utama.stok--;
				}
				aksi=0;
				system("clear");
			}
			if(aksi==2){//mandi
				if(utama.stat==1){
					utama.hygiene += 30;
					t=30;
					utama.stat = 0;
				}
				aksi=0;
				system("clear");
			}
			if(kondisi==1){
				while(status!=0){}
			}
			if(kondisi==2){
				while(status!=0){}
			}

			printf("Standby Mode\n");
			printf("Monster: %s\n",utama.nama);
			printf("Health: %d\n",utama.health);
			printf("Hunger: %d\n",utama.hunger);
			printf("Hygiene; %d\n",utama.hygiene);
			printf("Food left: %d\n",utama.stok);
			if(utama.stat==1) printf("Bath ready\n");
			else printf("Bath will be ready in %d s\n",t);

			printf("Choices\n");
			printf("1. Eat\n");
			printf("2. Bath\n");
			printf("3. Battle\n");
			printf("4. Shop\n");
			printf("5. Exit\n");

			//if(utama.stat==0) time--;

		}
		if(kondisi==1){//battle
			battle();
			kondisi=0;
		}
		if(kondisi==2){//shop
			shop();
			kondisi=0;
		}
		sleep(1);
		system("clear");
	}
}

void* waktu(void * args){
	while(1){
		if(x==1) break;
		globalTime++;
		if(utama.stat==0) t--;
		if(t==0) utama.stat=1;
		sleep(1);
	}
}

int main()
{
	int err1,err2,err3,err4;
	key_t key = 1111;
	shmid = shmget(key,sizeof(int),IPC_CREAT | 0666);
	stock = shmat(shmid,NULL,0);

	printf("Program Start...\n");
	*stock=5;
	x=0;//exit
	//struct monster utama;
	printf("Masukan Nama Monster Anda:");
	scanf("%s",utama.nama);
	globalTime=1;
	t = 0;
	aksi = 0;
	kondisi = 0;
	status =0;
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
	err3 = pthread_create(&(tid[2]),NULL,&waktu,NULL);
	if(err3) exit(EXIT_FAILURE);
	//err4 = pthread_create(&(tid[3]),NULL,&shop,NULL);
	//if(err4) exit(EXIT_FAILURE);


	pthread_join(tid[0],NULL);
	pthread_join(tid[1],NULL);
	pthread_join(tid[2],NULL);
	//pthread_join(tid[3],NULL);

	shmdt(stock);
	shmctl(shmid,IPC_RMID,NULL);
	return 0;
}
