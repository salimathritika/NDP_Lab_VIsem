//USING EVEN PARITY
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>

#define MAX 8

int main() {
    int recb, res,PORT;

    printf("Input PORT: ");
    scanf("%d", &PORT);

    struct sockaddr_in server;

    // Create Socket
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    sockfd < 0 ? perror("\nSocket creation error.") : printf("\nSocket created.");

    // Setup Server
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr("127.0.0.1");  // HOME
    server.sin_port = htons(PORT);
    printf("Ready to connect!");

    // Connecting
    connect(sockfd, (struct sockaddr*)&server, sizeof(server)) < 0 ? perror("\nConnection error.") : printf("\nSocket connected.");
    
    char buff[MAX+1];
    int one=0;
    printf("Enter a %d bit code:",(MAX-1));
    scanf("%s",buff);
    for(int i=0;i<MAX-1;i++)
    {
      if(buff[i]=='1')
         one++;
    }
    if(one%2!=0)
      buff[MAX-1]='1';
    else
      buff[MAX-1]='0';
    buff[MAX]='\0';
    res = send(sockfd, buff, sizeof(buff), 0);
    if (res < 0) {
        close(sockfd);
        printf("\nCode Sending Failed");
        exit(0);
    }
    printf("Code sent to the server\n");
    
    close(sockfd);
    
}
