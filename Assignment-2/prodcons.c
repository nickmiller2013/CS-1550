
/*Nick Miller
Project 2
CS 1550
February 23, 2016

gcc -m32 -o prodcons -I /u/OSLab/nam99/linux-2.6.23.1/include/ prodcons.c

*/
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h> 
#include <linux/unistd.h>

//Semaphore -- Wrapper class 
struct cs1550_sem{
	int value;
	//Some Process queue of your devising
	struct Node *first; //Start of linked list
	struct Node *last; //End of linked list
	
};

//Used to call up. 
void up(struct cs1550_sem *sem) {
       syscall(__NR_cs1550_up, sem);
}

//Used to call down
void down(struct cs1550_sem *sem) {
       syscall(__NR_cs1550_down, sem);
}

int main(int argc, char *argv[]) {

	int status; 
	//Checks for the right amount of arguments 
	if(argc < 4 || argc > 4) { 
		printf("There was a problem with your entered arguments, run program and try again.\n");
		return -1;
	}
	
	//Creating our alphabet 
	char alphabet[26] =  "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	
	//Setting the argument values 
	int numProd = atoi(argv[1]);
	int numCon = atoi(argv[2]);
	int bufferSize = atoi(argv[3]);
	
	//Declare the settings that you have 
	printf("The current settings are:\nProducer: %d\nConsumers:%d\nBuffer Size:%d\nWill wait 7 seconds then the program will run. \n",numProd,numCon, bufferSize);
	
	//printf("%d first %d two %d three",numProd, numCon, bufferSize);
	
	//Shared semaphores 
	struct cs1550_sem *empty, *full, *mutex;
	
	//Creating memory for the 3 shared semaphores, multiply by 3 for the 3 different semaphores. 
	empty = (struct cs1550_sem*) mmap(NULL, sizeof(struct cs1550_sem), PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANONYMOUS, 0, 0);
	full = (struct cs1550_sem*) mmap(NULL, sizeof(struct cs1550_sem), PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANONYMOUS, 0, 0);
	mutex = (struct cs1550_sem*) mmap(NULL, sizeof(struct cs1550_sem), PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANONYMOUS, 0, 0);

	//Now need to initialize the mutex. 
	//Empty initialize. 
	empty->first = NULL;
	empty->last = NULL;
	empty->value = bufferSize;
	
	//mutex initialize
	mutex->first = NULL;
	mutex->last = NULL;
	mutex->value = 1;

	//full initialize 
	full->first = NULL;
	full->last = NULL;
	full->value = 0;
	
	//in, out, and buffer positions --> From handouts. All pointers so they can be shared
	int *buffer, *in, *out;

	//Allocated space for the in position, out position, and buffer
	buffer = (int *)mmap(NULL, sizeof(int)*bufferSize, PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANONYMOUS, 0, 0);
	in = (int *)mmap(NULL, sizeof(int), PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANONYMOUS, 0, 0);
	out = (int *)mmap(NULL, sizeof(int), PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANONYMOUS, 0, 0);
	
	//Setting both in and out to 0. 
	*in = 0;
	*out = 0;
	
	sleep(7); 
	
	int i;
	//Increment like this so never exceeds given bound
	//Producer 
	for(i = 0; i < numProd; i++){
		if(fork() == 0){
			int pitem; 
			
			//Infinite Loop
			//This is almost straight from handout. 
			while(1){
				//Produce an item into pitem. 
				down(full);
				down(mutex); //locks the mutex 
				pitem = *in; 
				buffer[*in] = pitem;
				printf("Producer: %c Produced %d \n", alphabet[i], pitem);
				*in = (*in+1) % bufferSize; //incrementing
				up(mutex);
				up(full);
						
			}
		}
	}
	
	//Consumer 
	for(i = 0; i < numCon; i++){
		if(fork() == 0){
			int citem; 
			
			//Infinite Loop
			while(1){
				//Consume and item from citem 
				down(full);
				down(mutex); //Lock mutex 
				citem = buffer[*out];
				printf("Consumer: %c Consumed %d \n", alphabet[i], citem);
				*out = (*out + 1) % bufferSize;
				up(mutex);
				up(empty);	
			}
		}
	}			
				
	wait(&status); // Wait until all processes complete
	return 0; // Finished successfully
}