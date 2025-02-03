#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define PORT 8080
#define MAX_BUF_SIZE 1024

int main() {
    int sockfd;
    struct sockaddr_in server_addr;
    char buffer[MAX_BUF_SIZE];
    int option;
    char input_data[MAX_BUF_SIZE];

    // Create socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Socket creation failed");
        exit(1);
    }

    // Server address setup
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1"); // Assuming server is running locally

    // Connect to the server
    if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connection failed");
        close(sockfd);
        exit(1);
    }

    //while (1) {
        printf("Enter 1 to lookup IP from domain name\n");
        printf("Enter 2 to lookup domain from IP address\n");
        printf("Enter 0 to exit\n");
        printf("Choose an option: ");
        scanf("%d", &option);
        getchar(); // To consume the newline character

      /*  if (option == 0) {
            printf("Exiting...\n");
            break;
        }
*/
        printf("Enter your input: ");
        fgets(input_data, sizeof(input_data), stdin);
        input_data[strcspn(input_data, "\n")] = 0; // Remove newline character

        // Prepare message to send to server
        snprintf(buffer, sizeof(buffer), "%d%s", option, input_data);

        // Send request to server
        send(sockfd, buffer, strlen(buffer), 0);

        // Receive response from server
        memset(buffer, 0, sizeof(buffer));
        recv(sockfd, buffer, sizeof(buffer), 0);

        // Display the result
        printf("Server response: %s\n", buffer);
    //}

    // Close the socket
    close(sockfd);

    return 0;
}

