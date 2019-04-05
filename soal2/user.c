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
