#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> // used for closing file descriptor... here we have server_fd and client_fd after everything ...
#include <string.h> //CRUD strings , strlen, strcopy, strcat, strcmp

//Use of AF_INET?? AF_INET is a constant=2. we set address_family=2 -> imply IPV4. had it been IF_INET6, -> we are setting address family = 10 =>  IPv6 address family. address family is a instance attribute of sockaddr_ structure

//USE of struct sockaddr_in??
#include <netinet/in.h> // Internet address family. AF_INET, struct sockaddr_in


#include <sys/socket.h> //Socket APIs- Contains definitions. for socket-related functions and data structures like socket, bind, listen, accept, connect.. but what do you mean APIs?
//mysocket.bind
//mysocket.connect
//mysocket.accept

#define PORT 8080
#define MAX_BUFFER_SIZE 1024 // Maximum allowed size of message String

int main() {
    int tcp_server_fd;
    struct sockaddr_in address;
    
    // Creating socket file descriptor
    tcp_server_fd = socket(AF_INET, SOCK_STREAM, 0); //returns a unique int, a unique socket ID 
    //printf ("tcp_server_fd value =%d",tcp_server_fd);
    //int fd2=socket(AF_INET,SOCK_STREAM,0);
    //int fd3=socket(AF_INET,SOCK_STREAM,0);
    //int fd4=socket(AF_INET,SOCK_STREAM,0);
    //printf ("\nThe additional sockets have the file descriptor values %d,%d, and %d",fd2,fd3,fd4);
    
    //socket file descriptors are their ID. But if error occurs when calling socket(...) then...
    //. fd=negative values=error
    //. fd=0,1,2 are reserved for special cases
    //. fd=3,4,5,6.. is what we expect
    
    if (tcp_server_fd == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    // Setting up the server address
    //sin stands for service INPUT. eg: sin_port = service INPUT port.
    address.sin_family = AF_INET; // Address I'll be giving you is IPV4
    address.sin_addr.s_addr = INADDR_ANY;
    // sin_addr itself is actually a struct (template defined by <netinet/in.h>). 
    // anyway here is where we store our IPv4 address.
    address.sin_port = htons(PORT); 

    // Binding the socket to the specified address and port
    int server_bind_statuscode= bind(tcp_server_fd, (struct sockaddr *)&address, sizeof(address));
    //Parameters
    //1. ID
    //2. Pointer to the structure sockaddr_in that contains the IPADress and Port number
    //// we are typecasting sockaddr_in to sockaddr, apparently bind is a generic function to bind IPV4 or IPV6. But struct sockaddr_in is for IPV4 only, and sockaddr_in6 is for IPV6 only, thus to have a generic struct, we typecast it to (struct sockaddr *)
    //3. size of the struct addr_in....

    if (server_bind_statuscode< 0) {
        perror("bind");
        exit(EXIT_FAILURE);
    }
    
    // Listening for incoming connections
    int server_listen_statuscode=listen(tcp_server_fd, 3); 
    //This is NOT code blocker.
       
    
    if (server_listen_statuscode < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    
    printf("\nListener established...\n");
    
    
    // Accepting client connections------------------------------------------------
    
    int client_socket=socket(AF_INET,SOCK_STREAM,0);
    
    struct sockaddr_in client_address;
    
    int client_addrlen = sizeof(client_address);
    
    char buffer[MAX_BUFFER_SIZE] = {0};
    
    printf("\nWaiting for the client.\n");

    client_socket = accept(tcp_server_fd, (struct sockaddr *)&client_address, (socklen_t *)&client_addrlen);
    //THIS is the damn code blocker. which will not stop until some client actually wants to connect. if no client wants to connect, control flow will never go beyond this. The moment the first client is found, the socket API prolly has a queue that gets enqueued with the incoming clients details.
    
    // explaining the arguments... In bind fnc, we sent a pointer that points to the memory where server's IP address and port number is stored. Here we are creating space to store client_address and the len of client_address. We send the pointer to accept(), and the socket API will do the updating.  
    
    
    if (client_socket < 0) {
        perror("accept");
        exit(EXIT_FAILURE);
    }
    
    printf("\nClient Accepted. Listening on port %d...\n", PORT);


    // Receive data from the client and send a response
    while (1) {
        memset(&buffer[0], 0, sizeof(buffer));
        //Setting entire memory buffer=0
        // buffer looks like its  passed by value but no. buffer is a array, 
        // so writing buffer= &buffer[0]
        
        int bytes_received = recv(client_socket, &buffer[0], sizeof(buffer), 0);
        //int sockfd=client_socket
        //void *buf
        //size_t len = sizeof(buffer)
        //0 is the flag, meaning no flag is set.
        
        
        if (bytes_received <= 0) {
            // Connection closed or error occurred
            break;
        }

        printf("\nFrom client: %s", buffer);

        // Send a response back to the client
        char response[] = "Message received!";
        
    	send(client_socket,response,strlen(response),0);
    
    }

    close(client_socket);
    close(tcp_server_fd);
    return 0;
}

