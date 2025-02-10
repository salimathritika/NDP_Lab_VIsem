//WORKS
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>

#define MAX 8

// Error handling
void throwError(int sockfd, int newsockfd) {
    printf("\nMessage Recieving Failed");
    close(sockfd);
    close(newsockfd);
    exit(0);
}

int main() {
    int r, req, sntb, x, a = 0,one=0;
    struct sockaddr_in serv_addr, cli_addr;
    int PORT;
    printf("Input PORT: ");
    scanf("%d", &PORT);

    // creating a socket
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    sockfd < 0 ? perror("Error while creating socket\n") : printf("Socket Created!\n");

    // setup server
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(PORT);
    printf("Starting the Server\n");

    // binding with IP
    bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0 ? perror("Error while binding\n") : printf("Binding Successful\n");

    // listen to upto 5 clients
    listen(sockfd, 5);
    printf("Listening to PORT : %d...\n", PORT);

    // connecting to client
    int clength = sizeof(cli_addr);
    int newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clength);
    newsockfd < 0 ? perror("Error while accepting\n") : printf("Connection Established!\n");

    char buff[MAX];

    recv(newsockfd, buff, sizeof(buff), 0) < 0 ? throwError(sockfd, newsockfd) : printf("\nCode received!\n");
    
    for(int i=0;i<MAX;i++)
       {
         if(buff[i]=='1')
           one++;
       }
       
    if(one%2==0)
      printf("The code is not corrupted\n");
    else
      printf("The code is corrupted\n");
    close(sockfd);
    close(newsockfd);  
}

    

