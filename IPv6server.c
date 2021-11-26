/* 멀티 프로세스를 이용한 concurrent server:
클라이언트가 접속 (accept) 할때 서버가 자신은 계속 다른 클라이언트의 연결요청(connect)를 기다리고,
새로 자식 프로세스를 만들어 접속된 클라이언트들에 서비스를 제공 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>

#define TOTALFORK 3

int main(int argc, char* argv[]){
    if (argc != 3) {
        printf("usage: %s <IP> <Port no.>", argv[0]);
        exit(1);
    }

    int    sockfd, accsockfd;
    int    readlen = 0, sentlen = 0;
    struct sockaddr_in   cli4_addr;
    struct sockaddr_in6  serv_addr, cli6_addr;
    char   buffer[1024];
    pid_t  pid;

    /* create initial socket for client accept */
    sockfd = socket(AF_INET6, SOCK_STREAM, 0);

    /* create information of the server */
    memset(&serv_addr, 0x00, sizeof(serv_addr));
    serv_addr.sin6_family = AF_INET6;
    serv_addr.sin6_addr = in6addr_any;
    serv_addr.sin6_port = atoi(argv[2]);
    serv_addr.sin6_flowinfo = 0;

    /* bind the info to the socket */
    if (bind(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1){
        printf("error while executing bind() function!");
        exit(1);
    }

    /* listen to check the connect() request and create backlog queue*/
    listen(sockfd, 5);
    
    /* read the token from the client */
    int clientno = 0;
    while (1){
        /* accept for the actual communication with the client*/
        accsockfd = accept(sockfd, (struct sockaddr*)&cli6_addr, sizeof(cli6_addr));
        clientno += 1;
        printf("(%d) client connected...", clientno);
        pid = fork();

        /* child process handle the client */
        if (pid == 0){
            close(sockfd);
            while ((readlen = read(accsockfd, buffer, sizeof(buffer))) != 0){
                printf("%s", buffer);
            }
        }
        
    }


}