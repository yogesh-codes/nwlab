#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 1024
#define SERVER_IP "127.0.0.1"
#define PORT 8888

int main() {
    int clientSocket;
    struct sockaddr_in serverAddr;
    char buffer[BUF_SIZE];

    // Create socket
    clientSocket = socket(PF_INET, SOCK_DGRAM, 0);
    if (clientSocket == -1) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Configure server address
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = inet_addr(SERVER_IP);
    serverAddr.sin_port = htons(PORT);

    // Communicate with server
    while (1) {
        printf("Enter message: ");
        fgets(buffer, BUF_SIZE, stdin);

        // Send message to server
        if (sendto(clientSocket, buffer, strlen(buffer), 0, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1) {
            perror("sendto failed");
            break;
        }

        // Receive response from server
        if (recvfrom(clientSocket, buffer, BUF_SIZE, 0, NULL, NULL) == -1) {
            perror("recvfrom failed");
            break;
        }

        printf("Server response: %s\n", buffer);
    }

    // Close socket
    close(clientSocket);
    return 0;
}
