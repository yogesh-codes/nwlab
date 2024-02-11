#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#define PORT 8080
#define SERVER_IP "127.0.0.1" 

int main() {
    int tcp_client_fd;
    struct sockaddr_in server_address;
    char buffer[1024] = {0};

    // Creating socket file descriptor
    tcp_client_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (tcp_client_fd == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    // Setting up the server address to connect to
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);
    if (inet_pton(AF_INET, SERVER_IP, &server_address.sin_addr) <= 0) {
        perror("inet_pton");
        exit(EXIT_FAILURE);
    }

    // Connecting to the server
    if (connect(tcp_client_fd, (struct sockaddr *)&server_address, sizeof(server_address)) < 0) {
        perror("connect");
        exit(EXIT_FAILURE);
    }

    // Send and receive data with the server
    while (1) {
        printf("Send this message to server: ");
        fgets(buffer, sizeof(buffer), stdin);

        // Check if the user wants to exit
        if (strncmp(buffer, "exit", 4) == 0) {
            break;
        }

        // Send the message to the server
        send(tcp_client_fd, buffer, strlen(buffer), 0);

        // Receive and display the response from the server
        memset(buffer, 0, sizeof(buffer));
        recv(tcp_client_fd, buffer, sizeof(buffer), 0);
        printf("Server replied: %s", buffer);
    }

    close(tcp_client_fd);
    return 0;
}

