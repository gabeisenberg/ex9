#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define MAX_MSG_SIZE 1024

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <port>\n", argv[0]);
        return 1;
    }

    int port = atoi(argv[1]);
    int sockfd;
    struct sockaddr_in servaddr;
    char buffer[MAX_MSG_SIZE];

    // Create socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        perror("Socket creation failed");
        return 1;
    }

    // Initialize server address
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1"); // Server running on localhost
    servaddr.sin_port = htons(port);

    // Connect to server
    if (connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) != 0) {
        perror("Connection to server failed");
        return 1;
    }

    // Send message
    char message[] = "<Your Name>: <UFID>";
    if (send(sockfd, message, strlen(message), 0) == -1) {
        perror("Message sending failed");
        return 1;
    }

    // Receive message
    ssize_t bytes_received = recv(sockfd, buffer, MAX_MSG_SIZE - 1, 0);
    if (bytes_received == -1) {
        perror("Message receiving failed");
        return 1;
    }
    buffer[bytes_received] = '\0';

    printf("Server says: %s\n", buffer);

    // Close socket
    close(sockfd);

    return 0;
}
