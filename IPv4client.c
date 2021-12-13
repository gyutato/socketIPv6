#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/msg.h> 
#include <sys/ipc.h> 
#include "client.h"

typedef struct _msg_data {
   long  data_type;
   char  data_buff[256];
} msg_data;

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
    read(0, buffer, sizeof(buffer));
    write(sockfd,buffer,strlen(buffer));
    readfromS(sockfd);

    /* send port no. to server and receive next inquiry */
    memset(buffer, 0x00, sizeof(buffer));
    read(0, buffer, sizeof(buffer));
    write(sockfd, buffer, strlen(buffer));
    readfromS(sockfd);

    /* send Y for two times */
    memset(buffer, 0x00, sizeof(buffer));
    read(0, buffer, sizeof(buffer));
    write(sockfd, buffer, strlen(buffer));
    readfromS(sockfd);

    memset(buffer, 0x00, sizeof(buffer));
    read(0, buffer, sizeof(buffer));
    write(sockfd, buffer, strlen(buffer));
    readfromS(sockfd);

    /* send OK for final confirm */
    memset(buffer, 0x00, sizeof(buffer));
    read(0, buffer, sizeof(buffer));
    write(sockfd, buffer, strlen(buffer));
    read(sockfd, buffer, sizeof(buffer));

    /* prepare IPC with IPv6 server */
    printf("\n** waiting for IPC data **\n");
    sleep(20); //wait until the server receive all tokens

    key_t key = 7002;
    int msqid;
    msg_data data;

    if ((msqid = msgget(key, IPC_CREAT|0666)) == -1){
        perror("msgget() 실패: ");
        exit(1);
    }

    if(msgrcv(msqid, &data, sizeof(msg_data) - sizeof(long), 0, IPC_NOWAIT) == -1){
        perror ("msgrcv() 실패:");
        exit(1);
    }

    char recvtoken[256];
    memcpy(recvtoken, data.data_buff, sizeof(data.data_buff)-1);
    printf("\n 수신된 토큰: %s입니다.\n", recvtoken);
    printf("\n** Back to socket communication **\n");

    /* send received token to IPv4 server */
    recvtoken[strlen(recvtoken)] = 0x0a;
    write(sockfd, recvtoken, strlen(recvtoken));
    memset(buffer, 0x00, sizeof(buffer));
    readlen += read(sockfd, buffer, sizeof(buffer));
    printf("\n%s\n", buffer);



}