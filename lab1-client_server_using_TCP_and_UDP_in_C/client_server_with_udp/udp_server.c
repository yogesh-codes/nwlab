#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 1024
#define PORT 8888

int main() {
    int serverSocket;
    struct sockaddr_in serverAddr, clientAddr;
    char buffer[BUF_SIZE];
    socklen_t clientAddrSize;

    // Create socket
    serverSocket = socket(PF_INET, SOCK_DGRAM, 0);
    if (serverSocket == -1) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Configure server address
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddr.sin_port = htons(PORT);

    // Bind socket to the server address
    if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    while (1) {
        printf("Waiting for messages...\n");

        clientAddrSize = sizeof(clientAddr);
        // Receive message from client
        if (recvfrom(serverSocket, buffer, BUF_SIZE, 0, (struct sockaddr*)&clientAddr, &clientAddrSize) == -1) {
            perror("recvfrom failed");
            break;
        }

        printf("Received message: %s\n", buffer);

        // Send response back to client
        if (sendto(serverSocket, buffer, strlen(buffer), 0, (struct sockaddr*)&clientAddr, clientAddrSize) == -1) {
            perror("sendto failed");
            break;
        }
    }

    // Close socket
    close(serverSocket);
    return 0;
}
