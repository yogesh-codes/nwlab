#include <stdio.h>     
#include <stdlib.h>     
#include <string.h>     // for memset
#include <sys/socket.h> 
#include <netinet/in.h> // For struct sockaddr_in and INADDR_ANY
#include <arpa/inet.h>  // For inet_addr function
#include <unistd.h> 

#define PORT 8080


int sockfd;
struct sockaddr_in servaddr, cliaddr;


int main()
{
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
    perror("socket creation failed");
    exit(EXIT_FAILURE);
    }

    memset(&servaddr, 0, sizeof(servaddr));
    memset(&cliaddr, 0, sizeof(cliaddr));

    // Fill server information
    servaddr.sin_family = AF_INET; // IPv4
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(PORT); // Port number

    // Bind the socket with the server address
    if (bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    // Receive messages from client
    int len, n;
    len = sizeof(cliaddr); //len is value/resuslt
    n = recvfrom(sockfd, (char *)buffer, MAXLINE, MSG_WAITALL, (struct sockaddr *) &cliaddr, &len);
    buffer[n] = '\0';
    printf("Client : %s\n", buffer);

    // Send acknowledgement to client
    sendto(sockfd, (const char *)message, strlen(message), MSG_CONFIRM, (const struct sockaddr *) &cliaddr, len);

    // Close the socket
    close(sockfd);


    return 0;

}

main();
