#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>

#define PORT 1234
#define BUFFLEN 1024

int main(int argc, char** argv)
{

    if(argc != 2) {
        fprintf(stderr,"usage:\t%s\t<dest-IP>\n",argv[0]);
        exit(1);
    }

    int sockfd, newsockfd, n = 0;
    char sendline[BUFFLEN];
    char recvline[BUFFLEN];
    socklen_t len;
    struct sockaddr_in remote_addr;

    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket");
        exit(1);
    }

    memset(&remote_addr, 0, sizeof(remote_addr));
    remote_addr.sin_family = AF_INET;
    remote_addr.sin_port = htons(PORT);
    remote_addr.sin_addr.s_addr = inet_addr(argv[1]);

    if(bind(sockfd, (struct sockaddr*)&remote_addr, sizeof(remote_addr)) < 0) {
        perror("bind");
        exit(1);
    }

    if((newsockfd = connect(sockfd, (struct sockaddr*)&remote_addr, sizeof(remote_addr))) < 0) {
        perror("connect");
        exit(1);
    }

    while(1) {

        printf("$> ");
        fgets(sendline, BUFFLEN, stdin);
        if(strcmp(sendline,"close\n") == 0) {
            send(newsockfd, "close\n",strlen("close\n"),0);
            break;
        }

        send(newsockfd, sendline, strlen(sendline),0);
        n = recv(newsockfd, recvline, BUFFLEN, 0);
        recvline[n] = '\0';
        printf("From server:\t%s\n",recvline);

    }

    printf("Client closed\n");

    return EXIT_SUCCESS;
}