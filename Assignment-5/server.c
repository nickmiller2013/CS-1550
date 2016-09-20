
//	Nick Miller
//	Project 5
//  Due December 13th
//  Please read documentation. 
 

#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#define MYPORT 55024 //My port number
char *ipstring;
unsigned short port;


void toSend(char *forward, int value); //Sends to client
void toWrite(char *toward); //Writes out to stats.txt
void *toRespond(void *through); //This is the worker thread.

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

int main(){
    int ssock; //Socket id
    int value; //Slient id
    struct sockaddr_in addr;
    
    int thread_id; //When we run the thread.
    pthread_t thread;

    //This if statment checks the socket existence.
    if ((ssock = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket failed!\n");
        exit(EXIT_FAILURE);
    }
    
    //This is from Professor Wahns Lab.
    memset(&addr.sin_zero, 0, sizeof(addr.sin_zero));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(MYPORT);
    addr.sin_addr.s_addr = INADDR_ANY;

    //This is from Professor Wahns Lab.

    if (bind(ssock, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        perror("Bind failed!\n");
        exit(EXIT_FAILURE);
    }
    //This is from Professor Wahns Lab.

    if (listen(ssock, 10) < 0) {
        perror("listen failed!\n");
        exit(EXIT_FAILURE);
    }
    
    //Run for infinite loop because that was said to be okay to do.
    while(1){
        int listenfd, len;
        struct sockaddr_in in_addr;
        
        len = sizeof(in_addr);
        
        //Runs accept.
        value = accept(ssock, (struct sockaddr *)&in_addr, &len);
        
        //Checks to see if accept failed.
        if ((value) < 0) {
            perror("Accept failed!\n");
            exit(EXIT_FAILURE);
        }
        port = ntohs(in_addr.sin_port);  // ntohs converts from network endian order to host endian order
        ipstring = inet_ntoa(in_addr.sin_addr);  // inet_ntoa returns dotted string notation of 32-bit IP
        
        // Spawn worker thread
        thread_id = pthread_create(&thread, NULL, toRespond, (void *)&value);
    }
    
    close(ssock);
    return 0;
}

void toSend(char *forward, int value){
    int x = 0;
    while (x < strlen(forward)) {
        //Sending this information in forward to the client.
        int check1 = send(value, forward + x, strlen(forward) - x, 0);
        if(check1 < 0){
            
            perror("Send failed! Something weird must have happened. Proof.\n");
            exit(EXIT_FAILURE);
        }
        x += check1;
    }
}


    
    
    
void *toRespond(void *through){
    char *filename;
    char *temp;
    char *concatSender;
    
    char last[8]; //For the HTTP/1.1 check
    char sendThrough[1024]; //Max of 1024, this will be send to toSend
    char valid[1024]; //Check for more than 1024.
    
    int fSize = 0;
    int num1 = *(int *)through;

    int x = 0;
    
    //To figure out time and date.
    time_t nowTime;
    struct tm *time_struct;
    
    //This first if will be waiting for our message to arrive.
    if(recv(num1, valid, 1024, 0) > 0){
        //This if will make sure that the string being send is at least 16 charaters.
        if(strlen(valid) <= 15){
            temp = malloc(75);
            strcat(temp, "\nSorry but there must be at least 16 characters.\n");
            strcat(temp, "Lost connection.\n");
            toSend(temp, num1);
            free(temp);
       
            close(num1);
            
            //Deal with the mutex.
            pthread_mutex_lock(&mutex);
            
            //Write to the stats.txt that there was an invalid something.
            toWrite(valid);
            pthread_mutex_unlock(&mutex);
            
            return NULL;
        
        }
        
        //Now we check to make sure that our request begins with GET.
        //In here there are additions and subtractions from the places so we end up in the right place
        //When copying over things from the input.
        if(!strncmp("GET", valid,3)){
            filename = malloc(strlen(valid) - 14);
            strncpy(filename, valid + 4, strlen(valid) - 15); //Gets the filename to our variable filename
            toSend(filename, num1);
            strncpy(last, valid + 4 + strlen(filename) + 1, 8); //Gets the HTTP
            
            //This will make sure the user entered HTTP/1.1
            if(!strncmp(last, "HTTP/1.1", 8)){
                FILE *file = fopen(filename, "rt");
                
                //This will enter if the file is not found.
                if(file == NULL){
                    strcat(sendThrough, "\nHTTP/1.1 404 NOT FOUND\n");
                    toSend(sendThrough, num1);
                    close(num1);
                    
                    //Deal with mutex.
                    pthread_mutex_lock(&mutex);
                    //Write out our error.
                    toWrite(valid);
                    pthread_mutex_unlock(&mutex);
                    return NULL;
                }
                
                //Now we can start getting all our information and send it to our client.
                
                
                fseek(file,0,SEEK_END);
                fSize = ftell(file);
                fseek(file,0,SEEK_SET);
                
                strcat(sendThrough, "\nHTTP/1.1 200 OK\n"); //Start adding stuff to write out.

                nowTime = time(NULL);
                
                time_struct = gmtime(&nowTime);
                
                temp = malloc(50);
                
                strftime(temp, 50, "Date: %a, %d %b %Y %X %Z\n", time_struct );
                
                strcat(sendThrough, temp); //Add the time.
                
                strcat(sendThrough, "Content Length: ");
                
                sprintf(temp, "%d\n", fSize);	// convert file_size to string
                
                strcat(sendThrough, temp);
                
                strcat(sendThrough, "Connection: close\n");
                strcat(sendThrough, "Content Type: text/html\n\n");
                
                free(temp);
                
                //now we need to send our file through.
                temp = malloc(fSize);
                
                fread(temp, 1, fSize, file);
                
                fclose(file);
                
                concatSender = malloc(fSize);
                
                strcat(concatSender, temp);
                
                toSend(concatSender, num1);
                toSend(sendThrough, num1);
                
                
                
                
            }else{
                //Input didnt end with HTTP/1.1
                strcat(sendThrough, "\nFollow proper input please:\n");
                strcat(sendThrough, "GET /FILENAME.html HTTP/1.1\n");
                strcat(sendThrough, "Terminating connection.\n\n");
                
                toSend(sendThrough, num1);
            }
            
            
        }else {
            //Input didnt start with GET
            strcat(sendThrough, "\nPlease follow the proper format:\n");
            strcat(sendThrough, "GET /FILENAME.html HTTP/1.1\n");
            strcat(sendThrough, "Closing connection.\n\n");
            
            toSend(sendThrough, num1);
        }
        
    
        
        
        
        
    }
    close(num1); //close client connection.
    
    //Deal with mutex.
    pthread_mutex_lock(&mutex);
    
    //Print out valid.
    toWrite(valid);
    
    //Print out our client.
    toWrite("Client: ");
    toWrite(ipstring);
    toWrite(":55024\n");
    
    pthread_mutex_unlock(&mutex);
    
    
    
}

void toWrite(char *toward){
    FILE *f = fopen("stats.txt", "a+");
    
    if (f == NULL) {
        printf("Error opening stats.txt\n");
        
        return;
    }
    
    fseek(f, 0, SEEK_END);
    fwrite(toward, 1, strlen(toward), f);
    
    fclose(f);
    
}
