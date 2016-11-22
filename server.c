#include <stdio.h>
#include <ctype.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>


#define PORT 9001
#define QUEUE_MAX_COUNT 5
#define BUFF_SIZE 1024

#define SERVER_STRING "Server: hoohackhttpd/0.1.0\r\n"

int main(){
	int server_fd = -1;
	int client_fd = -1;

	u_short port=PORT;
	struct sockaddr_in client_addr;
	struct sockaddr_in server_addr;
        socklen_t client_addr_len = sizeof(client_addr);

	char buf[BUFF_SIZE];
	char recv_buf[BUFF_SIZE];
	char hello_str[]="hello,world!";

	int hello_len = 0;

	server_fd = socket(AF_INET,SOCK_STREAM,0);
	if(server_fd == -1){
		perror("socket");
		exit(-1);
	}

	memset(&server_addr,0,sizeof(server_addr));

	server_addr.sin_family = AF_INET;
	server_addr.sin_port=htons(PORT);
	server_addr.sin_addr.s_addr=htonl(INADDR_ANY);


	if(bind(server_fd,
				(struct sockaddr *)&server_addr,
				sizeof(server_addr))<0){
	perror("bind");
        exit(-1);	
	}


	if(listen(server_fd,QUEUE_MAX_COUNT)<0){
		perror("listen");
		exit(-1);	
	}

	printf("http server running on port %d\n", port);

	while (1) {
		client_fd = accept(server_fd, (struct sockaddr *)&client_addr,
					                      &client_addr_len);
		if (client_fd < 0) {
			perror("accept");
			exit(-1);}
		printf("accept a client\n");				    
		printf("client socket fd: %d\n", client_fd);

        hello_len = recv(client_fd, recv_buf, BUFF_SIZE, 0);
 
        printf("receive %d\n", hello_len);
 
        sprintf(buf, "HTTP/1.0 200 OK\r\n");
        send(client_fd, buf, strlen(buf), 0);
        strcpy(buf, SERVER_STRING);
        send(client_fd, buf, strlen(buf), 0);
        sprintf(buf, "Content-Type: text/html\r\n");
        send(client_fd, buf, strlen(buf), 0);
        strcpy(buf, "\r\n");
        send(client_fd, buf, strlen(buf), 0);
        sprintf(buf, "Hello World\r\n");
        send(client_fd, buf, strlen(buf), 0);
 
        close(client_fd);
    }
 
    close(server_fd);
 
    return 0;    

}
