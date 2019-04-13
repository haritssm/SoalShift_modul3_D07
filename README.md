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
Soal no 4, diharuskan untuk
