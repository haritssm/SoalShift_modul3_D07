#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#define PORT 8080

int x,shmid;
//key_t key = 1234;
int *stock;
int new_socket;
//char str[100]={0};

//int shmid = shmget(key, sizeof(int), IPC_CREAT | 0666);
//stock = shmat(shmid,NULL,0);
/*
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
		if(strcmp(str,"beli")==0){
			//(*stock)++;
			if(*stock<1) printf("Pembelian Gagal\n");
			else{
				printf("Pembelian Berhasil\n");
				(*stock)--;
			}
		}
		sleep(1);
	}
}*/

int main(int argc, char const *argv[]) {
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
    		address.sin_addr.s_addr = inet_addr("127.0.0.2");
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

		//key_t key = 1234;
		//shmid = shmget(key,sizeof(int),IPC_CREAT | 0666);
		//stock = shmat(shmid,NULL,0);
		/*
		pthread_t tid[2];
		int err1,err2;

		err1 = pthread_create(&(tid[0]),NULL,&print,NULL);
		if(err1) exit(EXIT_FAILURE);
		err2 = pthread_create(&(tid[1]),NULL,&in,NULL);
		if(err2) exit(EXIT_FAILURE);

		pthread_join(tid[0],NULL);
		pthread_join(tid[1],NULL);
		*/

		int valread;
		char str[100]={0};
		while(1){
			//scanf("%s",input);
			valread = read(new_socket,str,100);
			if(strcmp(str,"exit")==0){
				//x = 1;
				break;
			}
			if(strcmp(str,"exitexit")==0){
				x=1;
				break;
			}
			if(strcmp(str,"beli")==0){
				//(*stock)++;
				if(*stock<1) printf("Pembelian Gagal\n");
				else{
					printf("Pembelian Berhasil\n");
					(*stock)--;
				}
			}
			sleep(1);
		}

		/*
    		valread = read( new_socket , buffer, 1024);
    		printf("%s\n",buffer );
    		send(new_socket , hello , strlen(hello) , 0 );
    		printf("Hello message sent\n");
		*/
		close(server_fd);
		if(x==1) break;
	}
	shmdt(stock);
	shmctl(shmid, IPC_RMID, NULL);
    	return 0;
}
