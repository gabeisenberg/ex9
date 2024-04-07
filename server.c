#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define MAX_MSG_SIZE 1024
#define BACKLOG 5

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <port>\n", argv[0]);
        return 1;
    }

    int port = atoi(argv[1]);
    int listenfd, connfd;
    struct sockaddr_in servaddr, cliaddr;
    char buffer[MAX_MSG_SIZE];

    // Create socket
    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    if (listenfd == -1) {
        perror("Socket creation failed");
        return 1;
    }

    // Initialize server address
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(port);

    // Bind socket to address
    if (bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) != 0) {
        perror("Socket bind failed");
        return 1;
    }

    // Listen for connections
    if (listen(listenfd, BACKLOG) != 0) {
        perror("Listen failed");
        return 1;
    }

    printf("Server listening on port %d...\n", port);

    // Accept incoming connections
    socklen_t cliaddr_len = sizeof(cliaddr);
    connfd = accept(listenfd, (struct sockaddr *)&cliaddr, &cliaddr_len);
    if (connfd == -1) {
        perror("Connection accept failed");
        return 1;
    }

    // Receive message
    ssize_t bytes_received = recv(connfd, buffer, MAX_MSG_SIZE - 1, 0);
    if (bytes_received == -1) {
        perror("Message receiving failed");
        return 1;
    }
    buffer[bytes_received] = '\0';

    printf("Client says: %s\n", buffer);

    // Send message
    char message[] = "Welcome to the server running on REPTILIAN";
    if (send(connfd, message, strlen(message), 0) == -1) {
        perror("Message sending failed");
        return 1;
    }

    // Close sockets
    close(connfd);
    close(listenfd);

    return 0;
}
