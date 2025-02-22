#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <string.h>
#include <netinet/in.h>
#include <stdlib.h>

#define PORT 8080
#define MAX_BOOKS 100
#define BUFFER_SIZE 1024

typedef struct {
    char title[100];
    char author[100];
    int accession_number;
    int total_pages;
    char publisher[100];
} Book;

Book books[MAX_BOOKS];
int book_count = 0;

void insert_book(char *data, char *response) {
    if (book_count >= MAX_BOOKS) {
        strcpy(response, "Database full. Cannot insert more books.");
        return;
    }

    char *token = strtok(data, ",");
    strcpy(books[book_count].title, token);
    
    token = strtok(NULL, ",");
    strcpy(books[book_count].author, token);
    
    token = strtok(NULL, ",");
    books[book_count].accession_number = atoi(token);
    
    token = strtok(NULL, ",");
    books[book_count].total_pages = atoi(token);
    
    token = strtok(NULL, ",");
    strcpy(books[book_count].publisher, token);

    book_count++;
    strcpy(response, "Book inserted successfully.");
}

void delete_book(int accession_number, char *response) {
    int i, j;
    for (i = 0; i < book_count; i++) {
        if (books[i].accession_number == accession_number) {
            for (j = i; j < book_count - 1; j++) {
                books[j] = books[j + 1];
            }
            book_count--;
            strcpy(response, "Book deleted successfully.");
            return;
        }
    }
    strcpy(response, "Book not found.");
}

void display_books(char *response) {
    if (book_count == 0) {
        strcpy(response, "No books in the database.");
        return;
    }
    
    strcpy(response, "Book List:\n");
    char temp[BUFFER_SIZE];
    
    int i;
    for (i = 0; i < book_count; i++) {
        sprintf(temp, "Title: %s, Author: %s, Accession No: %d, Pages: %d, Publisher: %s\n",
                books[i].title, books[i].author, books[i].accession_number, books[i].total_pages, books[i].publisher);
        strcat(response, temp);
    }
}

void search_book(char *query, char *response) {
    strcpy(response, "Search Results:\n");
    
    int i;
    for (i = 0; i < book_count; i++) {
        if (strstr(books[i].title, query) != NULL || strstr(books[i].author, query) != NULL) {
            char temp[BUFFER_SIZE];
            sprintf(temp, "Title: %s, Author: %s, Accession No: %d, Pages: %d, Publisher: %s\n",
                    books[i].title, books[i].author, books[i].accession_number, books[i].total_pages, books[i].publisher);
            strcat(response, temp);
        }
    }

    if (strcmp(response, "Search Results:\n") == 0) {
        strcpy(response, "No book found.");
    }
}

int main() {
    int server_fd, client_fd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len = sizeof(client_addr);
    char buffer[BUFFER_SIZE], response[BUFFER_SIZE];

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
        perror("Socket creation failed");
        exit(1);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("Bind failed");
        exit(1);
    }

    if (listen(server_fd, 3) == -1) {
        perror("Listen failed");
        exit(1);
    }

    printf("Server is listening on port %d...\n", PORT);
    client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &addr_len);
    if (client_fd == -1) {
        perror("Accept failed");
        exit(1);
    }
    printf("Client connected.\n");

    while (1) {
        memset(buffer, 0, BUFFER_SIZE);
        recv(client_fd, buffer, BUFFER_SIZE, 0);

        memset(response, 0, BUFFER_SIZE);
        
        if (strcmp(buffer, "EXIT") == 0) {
            strcpy(response, "Server shutting down.");
            send(client_fd, response, strlen(response), 0);
            break;
        }

        if (strncmp(buffer, "INSERT ", 7) == 0) {
            insert_book(buffer + 7, response);
        } else if (strncmp(buffer, "DELETE ", 7) == 0) {
            int accession_number = atoi(buffer + 7);
            delete_book(accession_number, response);
        } else if (strcmp(buffer, "DISPLAY") == 0) {
            display_books(response);
        } else if (strncmp(buffer, "SEARCH ", 7) == 0) {
            search_book(buffer + 7, response);
        } else {
            strcpy(response, "Invalid command.");
        }
        
        send(client_fd, response, strlen(response), 0);
    }

    close(client_fd);
    close(server_fd);
    return 0;
}

