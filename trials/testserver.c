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
	} else
		printf("wating for the clients....\n");
 
	client_addr_len = sizeof(client_addr);
 
	while(1) {
		/* Do TCP handshake with client */
		client_sock_fd = accept(listen_sock_fd,
				(struct sockaddr*)&client_addr,
				&client_addr_len);
		if (client_sock_fd == -1) {
			perror("accept()");
			close(listen_sock_fd);
			return EXIT_FAILURE;
		}
 
		inet_ntop(AF_INET6, &(client_addr.sin6_addr),
				str_addr, sizeof(str_addr));
		printf("New connection from: %s:%d ...\n",
				str_addr,
				ntohs(client_addr.sin6_port));

         pid = fork();
        
         if (pid == 0){
            close(listen_sock_fd);

            memset(&buffer, 0x00, sizeof(buffer));
            read(client_sock_fd, buffer, sizeof(buffer));
            printf("%s\n", buffer);

            // memset(&buffer, 0x00, sizeof(buffer));
            // read(client_sock_fd, buffer, sizeof(buffer));
            // printf("%s\n", buffer);

            return EXIT_SUCCESS;
        } else {
            close(client_sock_fd);
        }
    }
    close(listen_sock_fd);
    return 0;
}




    //     /////////////////////////////////////////////////////////////////////////
	// 	/* Wait for data from client */
	// 	ret = read(client_sock_fd, &ch, 1);
	// 	if (ret == -1) {
	// 		perror("read()");
	// 		close(client_sock_fd);
	// 		continue;
	// 	}
 
	// 	/* Do very useful thing with received data :-) */
	// 	ch++;
 
	// 	/* Send response to client */
	// 	ret = write(client_sock_fd, &ch, 1);
	// 	if (ret == -1) {
	// 		perror("write()");
	// 		close(client_sock_fd);
	// 		continue;
	// 	}
 
	// 	/* Do TCP teardown */
	// 	ret = close(client_sock_fd);
	// 	if (ret == -1) {
	// 		perror("close()");
	// 		client_sock_fd = -1;
	// 	}
 
	// 	printf("Connection closed\n");
	// }
	// return EXIT_SUCCESS;
// }