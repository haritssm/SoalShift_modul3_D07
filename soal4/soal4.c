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
