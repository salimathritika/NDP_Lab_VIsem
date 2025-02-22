#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <string.h>
#include <netinet/in.h>
#include <stdlib.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    int client_fd;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE], response[BUFFER_SIZE];

    client_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (client_fd == -1) {
        perror("Socket creation failed");
        exit(1);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    if (connect(client_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("Connection failed");
        exit(1);
    }

    printf("Connected to server.\n");

    while (1) {
        printf("\nMenu:\n1. Insert Book\n2. Delete Book\n3. Display Books\n4. Search Book\n5. Exit\n");
        printf("Enter choice: ");
        int choice;
        scanf("%d", &choice);
        getchar();  

        memset(buffer, 0, BUFFER_SIZE);
        
        if (choice == 1) {
            char title[100], author[100], publisher[100];
            int accession_number, total_pages;

            printf("Enter title: ");
            fgets(title, 100, stdin); strtok(title, "\n");
            printf("Enter author: ");
            fgets(author, 100, stdin); strtok(author, "\n");
            printf("Enter accession number: ");
            scanf("%d", &accession_number);
            printf("Enter total pages: ");
            scanf("%d", &total_pages);
            getchar();
            printf("Enter publisher: ");
            fgets(publisher, 100, stdin); strtok(publisher, "\n");

            sprintf(buffer, "INSERT %s,%s,%d,%d,%s", title, author, accession_number, total_pages, publisher);
        } else if (choice == 2) {
            int accession_number;
            printf("Enter accession number of book to delete: ");
            scanf("%d", &accession_number);
            sprintf(buffer, "DELETE %d", accession_number);
        } else if (choice == 3) {
            strcpy(buffer, "DISPLAY");
        } else if (choice == 4) {
            char query[100];
            printf("Enter title or author to search: ");
            fgets(query, 100, stdin); strtok(query, "\n");
            sprintf(buffer, "SEARCH %s", query);
        } else if (choice == 5) {
            strcpy(buffer, "EXIT");
            send(client_fd, buffer, strlen(buffer), 0);
            recv(client_fd, response, BUFFER_SIZE, 0);
            printf("%s\n", response);
            break;
        } else {
            printf("Invalid choice. Try again.\n");
            continue;
        }

        send(client_fd, buffer, strlen(buffer), 0);
        memset(response, 0, BUFFER_SIZE);
        recv(client_fd, response, BUFFER_SIZE, 0);
        printf("%s\n", response);
    }

    close(client_fd);
    return 0;
}

