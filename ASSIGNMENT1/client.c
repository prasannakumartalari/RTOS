#include <netdb.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/socket.h> 
#define MAX 80 
#define PORT 8080 
#define SA struct sockaddr 
void func(int sockfd) 
{ 
	char buff[MAX]; 
	int n; 
	for (;;) { 
		bzero(buff, sizeof(buff)); 
		printf("Enter the string : "); 
		n = 0; 
		while ((buff[n++] = getchar()) != '\n') 
			; 
		write(sockfd, buff, sizeof(buff)); 
		bzero(buff, sizeof(buff)); 
		read(sockfd, buff, sizeof(buff)); 
		printf("From Server : %s", buff); 
		if ((strncmp(buff, "exit", 4)) == 0) { 
			printf("Client Exit...\n"); 
			break; 
		} 
	} 
} 

int main(int argc,char* argv[]) 
{ 
	int sockfd, connfd; 
	struct sockaddr_in servaddr, cli; 
        int server_port,server_ip;

	// socket create and varification 
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
        printf("%d\n",argc);
         //for (int i=0; i< argc; i++) {
     printf("%s", argv[2]);
 //}

    //server_ip = atoi(argv[1]); 
   // scanf("%d", &server_ip);   
	//servaddr.sin_addr.s_addr = inet_addr("127.0.0.1"); 
    servaddr.sin_addr.s_addr = inet_addr(argv[1]);
    server_port=atoi(argv[2]);    
   // scanf("%d", &server_port);

	servaddr.sin_port = htons(server_port); 

	// connect the client socket to server socket 
	if (connect(sockfd, (SA*)&servaddr, sizeof(servaddr)) != 0) { 
		printf("connection with the server failed...\n"); 
		exit(0); 
	} 
	else
		printf("connected to the server..\n"); 

	// function for chat 
	func(sockfd); 

	// close the socket 
	close(sockfd); 
} 

