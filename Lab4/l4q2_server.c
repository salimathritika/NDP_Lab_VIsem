//Exit not working
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define PORT 8080
#define MAX_BUF_SIZE 1024
#define DB_SIZE 3

// Define the structure for storing DNS records
typedef struct {
    char domain_name[50];
    char ip_address[16];
} DNSRecord;

// Predefined database of domain names and their corresponding IPs
DNSRecord database[DB_SIZE] = {
    {"www.google.com", "142.250.190.78"},
    {"www.facebook.com", "157.240.29.35"},
    {"www.github.com", "140.82.112.3"}
};

// Function to search for an IP address based on domain name
const char* lookup_ip(const char *domain_name) {
    for (int i = 0; i < DB_SIZE; i++) {
        if (strcmp(database[i].domain_name, domain_name) == 0) {
            return database[i].ip_address;
        }
    }
    return "Domain not found!";
}

// Function to search for a domain name based on IP address
const char* lookup_domain(const char *ip_address) {
    for (int i = 0; i < DB_SIZE; i++) {
        if (strcmp(database[i].ip_address, ip_address) == 0) {
            return database[i].domain_name;
        }
    }
    return "IP not found!";
}

int main() {
    int server_fd, client_fd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len;
    char buffer[MAX_BUF_SIZE];
    int option=1;

    // Create socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("Socket creation failed");
        exit(1);
    }

    // Server address setup
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    // Bind the socket
    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        close(server_fd);
        exit(1);
    }

    // Listen for incoming connections
    if (listen(server_fd, 5) < 0) {
        perror("Listen failed");
        close(server_fd);
        exit(1);
    }

    printf("Server is listening on port %d...\n", PORT);

    // Accept incoming connection from client
    client_len = sizeof(client_addr);
    client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &client_len);
    if (client_fd < 0) {
        perror("Accept failed");
        close(server_fd);
        exit(1);
    }

    printf("Client connected: %s\n", inet_ntoa(client_addr.sin_addr));

    // Communication with client
    //while (option!=0) {
        // Receive request from client
        memset(buffer, 0, sizeof(buffer));
        recv(client_fd, buffer, sizeof(buffer), 0);

        // Parse user option
        option = buffer[0] - '0'; // Convert char to int
        char *input_data = buffer + 1; // Domain name or IP address

        // If option is 0, exit the server
      /*  if (option == 0) {
            printf("Received exit request. Shutting down server...\n");
            close(client_fd);
    	    close(server_fd);

            return 0;
        }*/

        // Perform lookup based on the user's choice
        if (option == 1) {
            // Option 1: Get IP address from domain name
            const char *result = lookup_ip(input_data);
            send(client_fd, result, strlen(result), 0);
        } else if (option == 2) {
            // Option 2: Get domain name from IP address
            const char *result = lookup_domain(input_data);
            send(client_fd, result, strlen(result), 0);
        } else {
            printf("Invalid option!\n");
        }
    //}

    // Close the connection and server socket
    close(client_fd);
    close(server_fd);

    return 0;
}

