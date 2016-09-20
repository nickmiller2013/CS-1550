/*Nick Miller
  CS449
  Due Novem, 15
*/


#include "mymalloc.h"
#include <sys/mman.h> 
#include <stdio.h>

struct node *pass;
struct node *curr;

struct node *freelist[25];

int MAX_MEM = 1 << 30;
void *truthVal = NULL;

void combineIt(int);

int power(unsigned int x)
{
    int y = 0;
    while(x >>= 1){
        y++;
    }
    return y;
}

int sizeAccount(int size){
    if(size < 32){
        size = 32;
    }else{
        size++;
    }
    return size;
}

int whereBreak(int value2){
    int loop;
    for(loop = value2; loop < 25; loop++){
        if(freelist[loop] != NULL){
           // printf("\nCheck3");

            break;
        }
    }
    return loop;
}

//Does a series of checks to see where and how there needs to be divided
void *divider(int check,int value2){
    void *comeBack = NULL;

    struct node *curNode = freelist[check];

    
    if(check != 25){
        while(check > value2){
           // printf("\nDivinding in while loop with %d", check);

            freelist[check] = curNode->nextNode;

            
            curNode->key = check + 5;
            check--;
            
           //This is from the TA but I couldn't pass curNode for some reason.
            curNode->nextNode = ((void *)truthVal) + (((long long)((void *)curNode - ((void *)truthVal)))^((long long)(1 << (curNode->key))));
            
            
            curNode->nextNode->nextNode = freelist[check];

            
            curNode->nextNode->key = curNode->key;
            
            
            if(freelist[check]){
                //printf("\nCheck1");

                freelist[check]->prevNode = curNode->nextNode;
            }
            freelist[check] = curNode;
        }
        
        freelist[check] = curNode->nextNode;
        
        if(curNode->nextNode){
            //printf("\nCheck2");

            curNode->nextNode->prevNode = NULL;
        }
        curNode->key = curNode->key | 0x88;
        
        comeBack = curNode;	
        return comeBack++;
    }
    return NULL;
}

void *my_buddy_malloc(int size)
{
   // printf("\nAllocate");
	int i;
	
    //sets up truthVal
	if(truthVal == NULL){
		truthVal = mmap(NULL, MAX_MEM, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANON, 0, 0);
		freelist[24] = truthVal;
		freelist[24]->prevNode = NULL;
		freelist[24]->nextNode = NULL;
		freelist[24]->key = 30;
	}
    size = sizeAccount(size);
	
	int value2 = (int)(power(size) - 4);
	
    int check = whereBreak(value2);
    //printf("\nGot passed where Break");
	

	return divider(check,value2);
    //printf("\nGot passsed the divider");
}

struct node *calculateBuddy(struct node *curNode){
    return((void *)truthVal) + (((long long)((void *)curNode - ((void *)truthVal)))^((long long)(1 << (curNode->key)))); //Algorithm gotten by TA
}

void my_free(void *ptr)
{
    //printf("\nFree");
	ptr--;
    //printf("%d",ptr);
	if(*(char *)ptr & 0x88){
       // printf("\nGot into the if loop to free");
        curr = ptr;
        int value2;
        
        curr->key = curr->key & 0x7f;
        value2 = (curr->key & 0x7f) - 5;
			
        curr->nextNode = freelist[value2];
			
			
        if(freelist[value2]){
            curr->nextNode->prevNode = curr;
        }
        curr->prevNode = NULL;
			
        freelist[value2] = curr;
			
        pass = calculateBuddy(curr);
	
        combineIt(value2);
	}
}

void combineIt(int value2){
    while((value2 < 25) && ((pass->key & 0x80) == 0))
    {
        
        if(pass->prevNode != NULL){
            pass->prevNode->nextNode = pass->nextNode;
        }else{
            freelist[value2] = pass->nextNode;
        }
        
        if(curr->prevNode == NULL){
            freelist[value2] = curr->nextNode;
        }else{
            curr->prevNode->nextNode = curr->nextNode;
        }
        
        if(pass->nextNode){
            pass->nextNode->prevNode = pass->prevNode;
        }

        if(freelist[value2]){
            curr->nextNode->prevNode = curr;
        }
        
        if(pass < curr){
            curr = pass;
        }
        curr->nextNode = freelist[value2];

        curr->prevNode = NULL;
        
        freelist[value2] = curr;
        
        
    }
    
    pass = NULL;
    curr = NULL;
}




