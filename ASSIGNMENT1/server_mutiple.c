#include <signal.h>
#include <stdio.h> 
#include <netdb.h> 
#include <netinet/in.h> 
#include <stdlib.h> 
#include <pthread.h> 
#include <string.h> 
#include <sys/socket.h> 
#include <sys/types.h> 
#define MAX 80 
#define PORT 8080 
#define SA struct sockaddr 
int sockfd;

void handle_sigint(int sig) 
{ 
    printf("Caught signal %d\n", sig); 
    close(sockfd);
    exit(2);
} 
  

// Function designed for chat between client and server. 
void* func(void *arg) 
{ 
    void ** args=arg;
    int * sockfd = (int *)args[0];
    int client_id = args[1];
	char buff[MAX]; 
	int reciever_id=client_id; 
	// infinite loop for chat 
	for (;;) { 
		bzero(buff, MAX); 
            read(sockfd[client_id], buff, 6);
		// read the message from client and copy it in buffer 
		if(reciever_id==client_id){
        
        reciever_id=atoi(buff);
        continue; 
        }
         
		if(reciever_id!=client_id)
        write(sockfd[reciever_id], buff, strlen(buff)); 

		// if msg contains "Exit" then server exit and chat ended. 
		if (strncmp("exit", buff,4 ) == 0) { 
			printf("Server Exit...\n"); 
			break; 
		} 
	} 
} 

// Driver function 
int main(int argc,char* argv[]) 
{ 
	int connfd[100], len; 
	struct sockaddr_in servaddr, cli; 
    signal(SIGINT, handle_sigint); 

	// socket create and verification 
	sockfd = socket(AF_INET, SOCK_STREAM, 0); 
	if (sockfd == -1) { 
		printf("socket creation failed...\n"); 
		exit(0); 
	} 
	else
		printf("Socket successfully created..\n"); 
	bzero(&servaddr, sizeof(servaddr)); 

	// assign IP, PORT 
	servaddr.sin_family = AF_INET; 
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY); 
    int server_port=atoi(argv[1]);    
   // scanf("%d", &server_port);

	servaddr.sin_port = htons(server_port); 

	// Binding newly created socket to given IP and verification 
	if ((bind(sockfd, (SA*)&servaddr, sizeof(servaddr))) != 0) { 
		printf("socket bind failed...\n"); 
		exit(0); 
	} 
	else
		printf("Socket successfully binded..\n"); 

	// Now server is ready to listen and verification 
	if ((listen(sockfd, 5)) != 0) { 
		printf("Listen failed...\n"); 
		exit(0); 
	} 
	else
		printf("Server listening..\n"); 
	len = sizeof(cli); 

	// Accept the data packet from client and verification 
    int i=0;
    while(1) {
	connfd[i] = accept(sockfd, (SA*)&cli, &len); 
	if (connfd < 0) { 
		printf("server acccept failed...\n"); 
		exit(0); 
	} 
	else
		printf("server acccept the client...\n"); 

	// Function for chatting between client and server 
	//func(connfd); 
        pthread_t tid;
        void * args[2];
        args[0]=connfd;
        args[1]=i;    
        pthread_create(&tid,NULL,func,args);
    i++;
    }
	// After chatting close the socket 
	close(sockfd); 
} 

