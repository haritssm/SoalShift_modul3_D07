# SoalShift_modul3_D07

## Soal 1:
Pada soal no 1 ini, diharuskan untuk dapat menyelesaikan mencari suatu nilai faktorial dari list angka yang tidak urut dan dapat ditampilkan secari urut dari kecil ke besar. Proses untuk menghitung harus dapat dilakukan secara bersamaan.

## Penyelesaian:
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

## Source Code:
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
