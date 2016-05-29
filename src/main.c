#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>

#include "logging.h"

int create_socket()
{
    return socket(AF_INET, SOCK_STREAM, 0);
}

int bind_socket(int socket_fd, int port)
{
    struct sockaddr_in sock_addr;
    memset(&sock_addr, '0', sizeof(sock_addr));
    
    sock_addr.sin_family = AF_INET;
    sock_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    sock_addr.sin_port = htons(port);
    
    return bind(socket_fd, (struct sockaddr*) &sock_addr, sizeof(sock_addr));
}

int main(int argc, char *argv[])
{
    int port = 80;
    
    /* Parse argument flags */
    int i;
    for(i = 1; i < argc - 1; ++i){
        char *arg = argv[i];
        if(arg[0] == '-'){
            switch(arg[1]){
                case 'p': {
                    port = atoi(argv[++i]);
                }
            }
        }
    }
    
    /* Create server socket */
    int srv_sock = create_socket();
    if (srv_sock < 0) {
        log_print(ERROR, "Could not create server socket");
        exit(-1);
    }
    
    if (bind_socket(srv_sock, port) < 0) {
        log_print(ERROR, "Could not bind server socket");
        exit(-1);
    }
    
    /* Tell server to start listening */
    listen(srv_sock, 3);
    log_print(INFO, "Server online at port: %d", port);
    log_print(INFO, "Waiting for connection...");
    
    /* Create client socket fd */
    int sockfd;
    int c = sizeof(struct sockaddr_in);
    struct sockaddr_in client;
    
    /* Listen for connection */
    sockfd = accept(srv_sock, (struct sockaddr *) &client, (socklen_t *) &c);
    if (sockfd < 0) {
        log_print(ERROR, "Problem establishing connection");
        exit(-1);
    }
    
    log_print(INFO, "Connection successfully established");
    
    log_print(INFO, "Shutting down server");
    return 0;
}
