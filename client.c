#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define MAX_MSG_SIZE 1024

int main(int argc, char *argv[]) {
    if (argc != 2) {
        return 1;
    }

    int port = atoi(argv[1]);
    int sockfd;
    struct sockaddr_in servaddr;
    char buffer[MAX_MSG_SIZE];

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        return 1;
    }

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    servaddr.sin_port = htons(port);

    if (connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) != 0) {
        return 1;
    }

    char message[] = "Gabriel Isenberg: 27781379";
    if (send(sockfd, message, strlen(message), 0) == -1) {
        return 1;
    }

    ssize_t bytes_received = recv(sockfd, buffer, MAX_MSG_SIZE - 1, 0);
    if (bytes_received == -1) {
        return 1;
    }
    buffer[bytes_received] = '\0';

    printf(buffer);
    close(sockfd);
    return 0;
}
