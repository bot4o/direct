#include <sys/socket.h>
#include <string.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <poll.h>
#include <unistd.h>

#include <errno.h>

int main(void) {
    //socket(int domain, int type, int protocol);
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd < 0) {
        printf("Socket creation failed");  
        return 1;
    }
   
    struct sockaddr_in address = {
        AF_INET,
        htons(9999), //Номер на порта
        0 //IP address //0 = по default е localhost
    };

    //bind(sockfd, (struct sockaddr *)&address, sizeof(address)); 
    
    //SERVER ONLY 
    if (bind(sockfd, (struct sockaddr *)&address, sizeof(address)) == -1 ) {
        printf("Bind has failed! errno: %d\n", errno);
        return 1;
    }
    
    //SERVER ONLY 
    listen(sockfd, 10);
    
    //SEVER ONLY
    int clientfd = accept(sockfd, 0, 0);

    struct pollfd fds[2] =  {
        {
            //праща съобщение
            0,  //file descriptor
            POLLIN, //event
            0 //return event = pollin
        },
        {
            //получава съобщение
            clientfd,
            POLLIN,
            0
        }
    };
   
    for(;;) {
        char buffer[256] = { 0 };
        poll(fds, 2, 50000);

        if (fds[0].revents & POLLIN) {
            read(0, buffer, 255);
            char label[300] = "Server: ";
            strncat(label, buffer, sizeof(label) - strlen(label) - 1);
            send(clientfd, label, strlen(label), 0);
        } 
        else if (fds[1].revents & POLLIN) {
            if (recv(clientfd, buffer, 255, 0) == 0) {
                return 0;
            }
            printf("%s", buffer);
        }
    }

    return 0;
}
