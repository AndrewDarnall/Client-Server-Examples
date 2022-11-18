/**
 * @file server.c
 * @author Andrew R. Darnall
 * @brief 
 * @version 0.1
 * @date 2022-11-18
 * 
 * IPv4 TCP server (multiprocess)
 * 
 */


#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>

#define BUFFLEN 1024
#define PORT 1234
#define MAX_CONN 5

int createSocket(struct sockaddr_in *remote_addr)
{
    int sockfd;

    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket");
        exit(1);
    }

    memset(remote_addr, 0, sizeof(struct sockaddr_in));
    remote_addr->sin_family = AF_INET;
    remote_addr->sin_addr.s_addr = htonl(INADDR_ANY);
    remote_addr->sin_port = htons(PORT);

    if(bind(sockfd,(struct sockaddr*)remote_addr, sizeof(*remote_addr)) < 0) {
        perror("bind");
        exit(1);
    }

    return sockfd;
}

int main(int argc, char** argv)
{

    printf("-- Server process launched --\n");

    int sockfd, newsockfd, n = 0;
    struct sockaddr_in *remote_addr;
    socklen_t len;
    char sendline[BUFFLEN];
    char recvline[BUFFLEN];
    char command[BUFFLEN];

    strcpy(command,"start");

    sockfd = createSocket(remote_addr);

    listen(sockfd, MAX_CONN);


    while(1) {

        len = sizeof(*remote_addr);
        if((newsockfd = accept(sockfd, (struct sockaddr*)remote_addr, &len)) < 0) {
            perror("accept");
            exit(1);
        }

        if(fork() == 0) {

            close(sockfd);

            while(1) {

                //The look works for small ammounts of sent data, for larger ammounts use a loop!
                n = recv(newsockfd, recvline, BUFFLEN, 0);
                recvline[n] = '\0';
                if(strcmp(recvline,"close\n") == 0) break;
                printf("[%s]:\t%s",inet_ntoa(remote_addr->sin_addr),recvline);
                printf("Reply:> ");
                fgets(recvline,BUFFLEN,stdin);
                send(newsockfd, sendline, strlen(sendline),0);
                //The code to use telnet with it would look a little different because I'd have to send \r\n

            }


        } else {

            close(newsockfd);

        }

    }


    return EXIT_SUCCESS;
}