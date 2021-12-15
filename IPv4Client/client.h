#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>

char   buffer[1024];
int    readlen = 0, sockfd;

//왜 두 번에 걸쳐 읽어와야 하는지 의문. readlen을 출력하면 237인데?
void readfromS(int sockfd) {

    memset(buffer, 0x00, sizeof(buffer));
    readlen += read(sockfd, buffer, sizeof(buffer));
    printf("%s", buffer);

    memset(buffer, 0x00, sizeof(buffer));
    readlen += read(sockfd, buffer, sizeof(buffer));
    printf("%s", buffer);
}
