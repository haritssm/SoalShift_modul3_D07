# SoalShift_modul3_D07

## Soal 1:
Pada soal no 1 ini, diharuskan untuk dapat menyelesaikan mencari suatu nilai faktorial dari list angka yang tidak urut dan dapat ditampilkan secari urut dari kecil ke besar. Proses untuk menghitung harus dapat dilakukan secara bersamaan.

### Penyelesaian:
Untuk menyelesaikan soal no 1, dilakukan threading untuk menghitung nilai faktorial secara bersamaan. Pertama-tama dilakukan sorting dari argumen yaang diinputkan. Kami mensort argumen-argumen tersebut menggunakan bubble sort. Kelemahan dalam menggunakan bubble sort adalah jika jumlah input terlalu besar. 

```
void* sort(void* n) //sortnya pakai bubble sort
{
	int z = *(int *)n; //karena n tipenya void diubah menjadi integer
	int i, j; 
	for (i = 0; i < z-1; i++)
	{
       for (j = 0; j < z-i-1; j++)
	   {
           if (array[j] > array[j+1])
           {
           		int swap = array[j];
           		array[j]=array[j+1];
           		array[j+1]=swap;
		   }
       }
   }
}
```

Untuk mencari nilai faktorial kami menggunkan cara seperti biasa.
```
void* factorial(void* n) 
{
	int a = *(int *)n;
    int num=a;
        unsigned long long sum=1;
        while(a>0)
		{
            sum=sum*a;
            a--;
        }
        printf("%d! = %llu\n", num, sum);
}
```

### Source Code:
```
#include<stdio.h>
#include<string.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>

pthread_t thread;
pthread_t tid[50];

int array[50];

//sort
void* sort(void* n) //sortnya pakai bubble sort
{
	int z = *(int *)n; //karena n tipenya void diubah menjadi integer
	int i, j; 
	for (i = 0; i < z-1; i++)
	{
       for (j = 0; j < z-i-1; j++)
	   {
           if (array[j] > array[j+1])
           {
           		int swap = array[j];
           		array[j]=array[j+1];
           		array[j+1]=swap;
		   }
       }
   }
}

//factorial
void* factorial(void* n) 
{
	int a = *(int *)n;
    int num=a;
        unsigned long long sum=1;
        while(a>0)
		{
            sum=sum*a;
            a--;
        }
        printf("%d! = %llu\n", num, sum);
}

int main(int argc, char* argv[])
{
	int i;
    for(i=1; i<argc; i++)
	{
        array[i-1] = atoi(argv[i]); //masukin nilai argv ke dalam array
        							//pakai atoi karena argv tipenya char, diubah menjadi integer
    }
    
    int batas = argc-1;
    pthread_create (&thread, NULL, &sort, &batas);
    pthread_join(thread, NULL); //buat nunggu nge-sort dulu, baru di faktorial
    /*int y;
    for(y=0; y<batas; y++) printf("%d ", array[y]);
    printf("\n");*/
    for(i=0; i<argc-1; i++)
	{
    	pthread_create(&(tid[i]), NULL, &factorial, &array[i]);
    	pthread_join(tid[i], NULL); //buat nunggu faktorialnya selesai satu-satu
    }
	
}
```

## Soal 2:

Untuk soal 2, diharuskan untuk membuat soatu program server penjual dan pembeli yang dapat dihubungkan oleh user untuk mengakses suatu stok barang yang merupakan variable shared memory. Jika user terhubung dengan server penjual user dapat menambahkan stok barang dan server dapat menampilkan stock barang setiap 5 menit. Jika user terhubung ke server pembeli, user dapat membeli stok yang akan mengurangkan nilai dar istok barang.

### Penyelesaian:
Untuk setiap server digunakan socket dengan nilai address yang berbeda. Untuk stok barang digunakan shared memory agar naila stok dapat diakses dari server pembeli dan penjual.

```
	int server_fd;
    	struct sockaddr_in address;
    	int opt = 1;
    	int addrlen = sizeof(address);
    	//char buffer[1024] = {0};
    	//char *hello = "Hello from server";
	//stock = &val;
	key_t key = 1234;
	shmid = shmget(key,sizeof(int),IPC_CREAT | 0666);
	stock = shmat(shmid,NULL,0);

	while(1){
		x=0;
		printf("Waiting for connection...\n");
    		if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        		perror("socket failed");
        		exit(EXIT_FAILURE);
    		}

    		if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        		perror("setsockopt");
        		exit(EXIT_FAILURE);
    		}

    		address.sin_family = AF_INET;
    		address.sin_addr.s_addr = inet_addr("127.0.0.1");
    		address.sin_port = htons( PORT );

    		if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0) {
        		perror("bind failed");
        		exit(EXIT_FAILURE);
    		}

    		if (listen(server_fd, 3) < 0) {
        		perror("listen");
        		exit(EXIT_FAILURE);
    		}

    		if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0) {
        		perror("accept");
        		exit(EXIT_FAILURE);
    		}
		printf("Connected to user!\n");
		.
		.
		.
```

Fitur pada server Penjual adalah menambahkan stok barang dan juga dapat menampilkan stok barang tersebut setiap 5 menit. Untuk dapat melakukan hal tersebut secara bersamaan adalah dengan menggunakan thread. Untuk menambahkan stok adalah dengan input berupa "tambah" dari program user.
```
// membaca input dari user
void* in(void *arg){
	int valread;
	//char str[100]={0};
	while(1){
		//scanf("%s",input);
		valread = read(new_socket,str,100);
		if(strcmp(str,"exit")==0){
			x = 1;
			break;
		}
		if(strcmp(str,"exitexit")==0){
			x=2;
			break;
		}
		if(strcmp(str,"tambah")==0){
			(*stock)++;
		}
		sleep(1);
	}
}

```
```
void* print(void *arg){
	int valread;
	//char str[100]={0};
	while(1){
		//valread = read(new_socket,str,100);
		if(x==1 || x==2) break;
		printf("Stock Barang: %d\n", *stock);
		sleep(5);
	}
}
```

Untuk bagian user, user dapat memilih server mana yang akan terkoneksi dengan cara menginputkan addressnya. Setelah terkoneksi, user dapat menginputkan "tambah" untuk menambah stok barang, "beli" untuk mengurangi stok barang.
```
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#define PORT 8080

int main(int argc, char const *argv[]) {
    	struct sockaddr_in address;
    	int sock = 0, valread,x=0;
    	struct sockaddr_in serv_addr;
    	char *arg,str[100],ad[10];
    	//char buffer[1024] = {0};
	while(1){
		printf("Connecting to server...\n");
		scanf("%s",ad);
    		if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        		printf("\n Socket creation error \n");
        		return -1;
    		}

    		memset(&serv_addr, '0', sizeof(serv_addr));

    		serv_addr.sin_family = AF_INET;
    		serv_addr.sin_port = htons(PORT);

		//scanf("%s",ad);

    		if(inet_pton(AF_INET, ad, &serv_addr.sin_addr)<=0) {
        		printf("\nInvalid address/ Address not supported \n");
        		return -1;
    		}

    		if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        		printf("\nConnection Failed \n");
        		return -1;
    		}
		if(strcmp(ad,"127.0.0.1")==0) printf("Connected to server Penjual\n");
		if(strcmp(ad,"127.0.0.2")==0) printf("Connected to server Pembeli\n");

		//printf("Connected to server!\n");

		while(1){
			scanf("%s",str);
			arg = str;
			if(strcmp(str,"tambah")==0 && strcmp(ad,"127.0.0.1")==0){
				send(sock,arg,strlen(arg),0);
			}
			if(strcmp(str,"beli")==0 && strcmp(ad,"127.0.0.2")==0){
				send(sock,arg,strlen(arg),0);
			}
			if(strcmp(str,"exit")==0){
				send(sock,arg,strlen(arg),0);
				break;
			}
			if(strcmp(str,"exitexit")==0){
				send(sock,arg,strlen(arg),0);
				x=1;
				break;
			}
		}
		if(x==1){
			x=0; 
			break;
		}
	}

	/*
    	send(sock , hello , strlen(hello) , 0 );
    	printf("Hello message sent\n");
    	valread = read( sock , buffer, 1024);
    	printf("%s\n",buffer );
	*/
    	return 0;
}
```

## Soal 3:
Pada soal no 3, diharuskan untuk membuat program yang dapat melakukan beberapa fitur secara bersamaan. Soal ini dikemas dalam bentuk cerita untuk membuat yang lain tidur atau yang lainnya untuk bangun.

### Penyelesaian:
Kami membuat 4 thread yang memiliki kegunaan seperti membaca input, status Agmal, status Iraj, dan untuk menghentikan program. Input akan dilakukan secara terus menerus. Jika input merupakan "All Status" maka akan diprint status dari Agmal dan Iraj. Jika "Agmal Ayo Bangun" akan menjalakan fungsi pada thread A (menambahkan status Agmal) dan "Iraj Ayo Tidur" akan dijalankan B (mengurangi status pada Iraj).
```
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
```

Proses diatas dilakukan menggunakan thread.

## Soal 4:
Soal no 4, diharuskan untuk membuat program yang membuat data log dari ps aux sebanyak 10 baris. Log tersebut harus dibuat dalam 2 file dan masing-masing didalam file yang berbeda. Setelah itu mengcompress kedua file dan menghapus file. Setelah itu mengunzip kembali kedua file. Proses harus dapat secara bersamaan.

### Penyelesaian:
Pertama-tama, kami menggunakan popen("ps aux | tail -n 10","r") untuk mebaca log dan memasukannya dalam suatu variable. Setelah itu variable tersebut menjadi input argumen pada thread. Dilakukan 2 thread. Proses pada thread pertama membuat file masing-masing denga nama "SimpanProses#.txt" dan disimpan dalam folder "FolderProses#". Setelah itu dizip dalam nama file "KompresProses#.zip" dan menghapus file dan mengunzipnya kembali.

### Source Code:
```
#include<stdio.h>
#include<string.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>

pthread_t tid[2]; //inisialisasi array untuk menampung thread dalam kasus ini ada 2 thread
//int counter = 0;
//int length=5; //inisialisasi jumlah untuk looping

struct arg{
	char str[1000];
	int counter;
};

void* proses(void *args)
{
	FILE *f;
	struct arg *ar = (struct arg *)args;
	char fl[100];
	sprintf(fl,"/home/zicoritonda/Documents/FolderProses%d/SimpanProses%d.txt",ar->counter,ar->counter);
	f = fopen(fl,"w");
	fprintf(f,"%s",ar->str);
	fclose(f);

	sprintf(fl,"cd /home/zicoritonda/Documents/FolderProses%d; zip KompresProses%d.zip SimpanProses%d.txt",ar->counter,ar->counter,ar->counter);
	system(fl);

	sprintf(fl,"cd /home/zicoritonda/Documents/FolderProses%d; rm SimpanProses%d.txt",ar->counter,ar->counter);
	system(fl);

	printf("Wait 15 sec\n");
	sleep(15);
	sprintf(fl,"cd /home/zicoritonda/Documents/FolderProses%d; unzip KompresProses%d.zip",ar->counter,ar->counter);
	system(fl);

	//counter++;
}

int main()
{
	struct arg *arg1,*arg2,args1,args2;
	arg1 = &args1;
	arg2 = &args2;

	//args1.str = "";
	//args2.str = "";

	int i=0;
	int err1,err2;
 	FILE *f;
	//char *arg;
	char str[100];
	//args1.str[0]="";
	//args2.str[0]="";
	strcpy(args1.str,"");
	strcpy(args2.str,"");

	//arg1->str = malloc(sizeof(char) * 800);
	//arg2->str = malloc(sizeof(char) * 800);
	f = popen("ps aux | tail -n 10","r");
	while(fgets(str,sizeof(str),f) != NULL){
		strcat(args1.str,str);
		strcat(args2.str,str);
	}
	pclose(f);
	args1.counter = 1;
	args2.counter = 2;

	printf("%d\n%s\n",args1.counter,args1.str);
	printf("%d\n%s\n",args2.counter,args2.str);

	err1 = pthread_create(&(tid[0]),NULL,&proses,(void*) arg1);
	if(err1) exit(EXIT_FAILURE);
	err2 = pthread_create(&(tid[1]),NULL,&proses,(void*) arg2);
	if(err2) exit(EXIT_FAILURE);

	printf("create thread 1 return: %d\n",err1);
	printf("create thread 2 return: %d\n",err2);

	pthread_join(tid[0],NULL);
	pthread_join(tid[1],NULL);

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
```

## Soal 5:
Pad soal 5 merupakan suatu game survival untuk dapat merawat suatu monster agar dapat terus bertahan dengan menjaga health, hunger, hygiene monster tersebut. Terdapat fitur battle denagn mosnter lain dan juga fitur shop untuk membeli stok makanan. Soal tambahan adalah bagaiman dapat membuat program yang inputnya tidak memerlukan enter.

### Penyelesaian:
Untuk input secara key press yang mencari sumber open source pada internet yaitu dengan membuat suatu fungsi.
```
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
```

Untuk melakukan semua fitur tersebut kami melakukannya secara threading. Pertama untuk menerima input dan juga untuk kondisi standby pada game dan menghitung waktu sercara global. Input merupakan angka pilihan pada menu.
```
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
```

Pada fitur standby akan menampilkan status pada monster dan juga mengurangi nilai status sesuai per beberap detik.
```
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
```

Selanjutnya terdapat thread untuk menghitung secara global.
```
void* waktu(void * args){
	while(1){
		if(x==1) break;
		globalTime++;
		if(utama.stat==0) t--;
		if(t==0) utama.stat=1;
		sleep(1);
	}
}
```

Jika input untuk memilih mode battle akan dijalankan fungsi battle.
```
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
```

Untuk input memilih fitur shop.
```
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
```
