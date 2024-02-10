#include <stdio.h>     
#include <stdlib.h>     
#include <string.h>     // for memset
#include <sys/socket.h> 
#include <netinet/in.h> // For struct sockaddr_in and INADDR_ANY
#include <arpa/inet.h>  // For inet_addr function
#include <unistd.h> 

#define PORT 8080


int sockfd;
struct sockaddr_in servaddr;


// Create socket
void client()

{
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&servaddr, 0, sizeof(servaddr));

    // Server information
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    // Send message to server
    sendto(sockfd, (const char *)message, strlen(message), MSG_CONFIRM, (const struct sockaddr *) &servaddr, sizeof(servaddr));

    // Receive server response
    n = recvfrom(sockfd, (char *)buffer, MAXLINE, MSG_WAITALL, (struct sockaddr *) &servaddr, &len);
    buffer[n] = '\0';
    printf("Server : %s\n", buffer);

    // Close the socket
    close(sockfd);

}

main();