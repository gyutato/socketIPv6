#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include "client.h"

int main(int argc, char* argv[]){
    if (argc != 3) {
        printf("usage: %s <IP> <Port no.>", argv[0]);
        exit(1);
    }

    int    sockfd, read_len, sentlen;
    int    idx = 0, readlen = 0;
    struct sockaddr_in serv_addr;
    char   buffer[1024];

    /* create a socket for the client */
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1){
        printf("error while creating client socket!");
        exit(1);
    }

    /* create a struct for the server information */
    memset(&serv_addr, 0x00, sizeof(struct sockaddr_in));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_addr.sin_port = htons(atoi(argv[2]));

    /* connect the IPv4 server */
    if (connect(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1){
        printf("error while connecting the server!");
        exit(1);
    }

    /* receive the initial message from the server */
    readfromS(sockfd);

    /* send my stdID to server and receive next inquiry */
    memset(buffer, 0x00, sizeof(buffer));
    read(0,buffer,sizeof(buffer)); //fgets-fputs 방식의 경우 %s 뒤의 개행문자로 인해 입력이 종료됨 (서버가 반응할 수 없음)
    write(sockfd,buffer,strlen(buffer));
    readfromS(sockfd);

    /* send my IPv6 addr to server and receive next inquiry */
    memset(buffer, 0x00, sizeof(buffer));
    read(0,buffer,sizeof(buffer));
    write(sockfd,buffer,strlen(buffer));
    readfromS(sockfd);

    /* send port no. to server and receive next inquiry */
    memset(buffer, 0x00, sizeof(buffer));
    read(0,buffer,sizeof(buffer));
    write(sockfd,buffer,strlen(buffer));
    readfromS(sockfd);

}