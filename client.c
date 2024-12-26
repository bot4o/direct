#include <sys/socket.h>
#include <string.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <poll.h>
#include <unistd.h>

int main(void) {
    //socket(int domain, int type, int protocol);
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd < 0) {
        printf("failed");  
        return 1;
    }

    struct sockaddr_in address = {
        AF_INET,
        htons(9999),
        0
    };
    //CLIENT ONLY    
    connect(sockfd, (struct sockaddr *)&address, sizeof(address));
    //stdin - 0 "input from the user"
    struct pollfd fds[2] =  {
        {
            0,
            POLLIN,
            0
        },
        {
            sockfd,
            POLLIN,
            0
        }
    };
   
    for(;;) {
        char buffer[256] = { 0 };
        poll(fds, 2, 50000);

        if (fds[0].revents & POLLIN) {
            read(0, buffer, 255);
            char labeled_message[300] = "Client: ";
            strncat(labeled_message, buffer, sizeof(labeled_message) - strlen(labeled_message) - 1);

            send(sockfd, labeled_message, strlen(labeled_message), 0);
        } 
        else if (fds[1].revents & POLLIN) {
            if (recv(sockfd, buffer, 255, 0) == 0) {
                return 0;
            }
            printf("%s", buffer);
        }
    }

    return 0;
}
