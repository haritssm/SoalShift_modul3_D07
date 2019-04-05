#include<stdio.h>
#include<string.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>

pthread_t tid[3]; //inisialisasi array untuk menampung thread dalam kasus ini ada 2 thread

int length=5; //inisialisasi jumlah untuk looping

//sort
void swap(int *xp, int *yp) 
{ 
    int temp = *xp; 
    *xp = *yp; 
    *yp = temp; 
} 
 
void sort(int arr[], int n) 
{
   int i, j; 
   for (i = 0; i < n-1; i++){    
       for (j = 0; j < n-i-1; j++){
           if (arr[j] > arr[j+1])
              swap(&arr[j], &arr[j+1]);
       }
   }
}

//factorial
unsigned int* factorial(unsigned int n) 
{
    if (n == 0) 
    return 1; 
    return n * factorial(n - 1);
}

int main(int argc, char* argv[])
{
	int array[50];
    for(i=1; i<argc; i++){
        array[i]= atoi(argv[i]);
    }
    
    sort(array, argc);
    //while(i--){
    //    printf("%d", array[i]);
    //    }

    for(i=1; i<argc; i++){
        printf("%ud", factorial(array[i]))
    }
}
