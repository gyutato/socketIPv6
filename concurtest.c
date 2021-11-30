#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <netinet/in.h>
 
#define CLIENT_QUEUE_LEN 10
#define SERVER_PORT 7002
 
int main(void)
{
	int listen_sock_fd = -1, client_sock_fd = -1;
	struct sockaddr_in6 server_addr, client_addr;
	socklen_t client_addr_len;
	char str_addr[INET6_ADDRSTRLEN], pton_addr[16], buffer[1024];
	int ret, flag;
	char ch;
    pid_t pid;
 
	/* Create socket for listening (client requests) */
	listen_sock_fd = socket(AF_INET6, SOCK_STREAM, IPPROTO_TCP);
	if(listen_sock_fd == -1) {
		perror("socket()");
		return EXIT_FAILURE;
	} else
		printf("socket created....\n");
 
	/* Set socket information */
	server_addr.sin6_family = AF_INET6;
	// server_addr.sin6_addr = in6addr_any;
    inet_pton(AF_INET6, "2001:0:c38c:c38c:4c2:3c8a:503f:828", (void*)&pton_addr);
    memcpy((void*)&server_addr.sin6_addr.s6_addr, (void*)pton_addr, 16);
	server_addr.sin6_port = htons(SERVER_PORT);
 
	/* Bind address and socket together */
	ret = bind(listen_sock_fd, (struct sockaddr*)&server_addr, sizeof(server_addr));
	if(ret == -1) {
		perror("bind()");
		close(listen_sock_fd);
		return EXIT_FAILURE;
	} else
		printf("binding the socket....\n");
 
	/* Create listening queue (client requests) */
	ret = listen(listen_sock_fd, CLIENT_QUEUE_LEN);
	if (ret == -1) {
		perror("listen()");
		close(listen_sock_fd);
		return EXIT_FAILURE;
	} 
	client_addr_len = sizeof(client_addr);

 while(1)
 {
  //parent process waiting to accept a new connection
  printf("\n*****server waiting for new client connection:*****\n");
  client_sock_fd=accept(listen_sock_fd, (struct sockaddr*)&client_addr, &client_addr_len);
  inet_ntop(AF_INET6, &(client_addr.sin6_addr), str_addr, sizeof(str_addr));
  printf("New connection from: %s:%d ...\n", str_addr, ntohs(client_addr.sin6_port));

  //child process is created for serving each new clients
  pid=fork();

  if(pid==0)//child process read and write..
  {
   //rceive from client
   while(1)
   {
    memset(buffer, 0x00, sizeof(buffer));
    int readlen = read(client_sock_fd, buffer, sizeof(buffer));

    if(readlen == 0)
    {
     close(client_sock_fd);
     break;
    }

    buffer[readlen] = 0;
    send(client_sock_fd, buffer, readlen,0);
    printf("%s\n",buffer);
   }//close interior while
  exit(0);
  }
  else
  {
   close(client_sock_fd);//sock is closed BY PARENT
  }
 }//close exterior while

 return 0;
}