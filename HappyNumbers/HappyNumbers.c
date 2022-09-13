#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdint.h>

// Global Variables
int happyNums = 0;
int limit;
int threadCount;
pthread_mutex_t myLock;

// Gets the sum of all the digits squared and returns it

int getDigitSum(int num){
  int total=0;
  int digit=0;
  while(num > 0){
    digit = num % 10;
    total += digit * digit;
    num /= 10;
    }
  return total;
}

// Calculates the number of happy numbers from 1 to a specified limit
// the parameter is a pointer to the number of the pointer 

void *happyNumbers(void* arg){
  int index = (intptr_t)arg + 1;
  while(index <= limit){
  int total = index;
while(total != 1 && total != 4){ 
  total = getDigitSum(total);
}
if(total == 1){
pthread_mutex_lock(&myLock);
happyNums++;
pthread_mutex_unlock(&myLock);
}
index += threadCount;
  }
return NULL;
}

int main(int argc, char *argv[]) {

  // Error checking for the number of arguments, if the thread count is valid,
  // and if the limit is valid and > 100
  if(argc != 5){
    printf("Error, invalid command line arguments\n");
    return 0;
  }
  threadCount = atoi(argv[2]);
  if(threadCount < 1 || threadCount > 4){
    printf("Error, invalid thread count\n");
    return 0;
  }

  limit = atoi(argv[4]);
  if(limit < 1){
    printf("Error, invalid limit value\n");
    return 0;
  }

  if(limit < 100){
    printf("Error, limit must be > 100\n");
    return 0;
  }
  
  // Initialize the error variables and the pthread array of size threadCount
  unsigned long int tError, mtxErr;
  pthread_t threads[threadCount];

  // Initalize the lock 
  mtxErr = pthread_mutex_init(&myLock,NULL);
  if (mtxErr != 0)
      printf("Mutex Initialization Failed");

  // Creates the threads based on the thread count
  for(int i=0;i<threadCount;i++){
    tError = pthread_create(&threads[i],NULL,&happyNumbers,(void*)(intptr_t)i);
    
  if (tError != 0)
      printf("Thread %d fail to create.\n", i);
  }
  // Waits for all of the threads to complete
  for(int i=0;i<threadCount;i++){
    pthread_join(threads[i],NULL);
  }
  
  // Prints out the result
  printf("Counting Happy and Sad Numbers from 1 to %d\n", limit);
  printf("Please wait. Running...\n\n");

printf("Happy Nums: %d\n", happyNums);
int sadNums = limit - happyNums;
printf("Sad Nums: %d\n", sadNums);

return 0;
}
