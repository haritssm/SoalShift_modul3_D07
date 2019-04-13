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

