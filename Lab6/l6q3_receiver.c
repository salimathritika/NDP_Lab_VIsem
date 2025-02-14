#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 8080
#define DATA_BITS 4  // Change this as needed

// Function to detect and correct errors
int correctHammingCode(int hamming[], int dataSize) {
    int parityCount = 0, i, j, k, errorPos = 0;

    while ((1 << parityCount) < (dataSize + parityCount + 1)) {
        parityCount++;
    }

    int totalSize = dataSize + parityCount;

    // Check for errors
    for (i = 0; i < parityCount; i++) {
        int pos = (1 << i);
        int parity = 0;
        for (j = pos - 1; j < totalSize; j += (pos * 2)) {
            for (k = 0; k < pos && (j + k) < totalSize; k++) {
                parity ^= hamming[j + k];
            }
        }
        if (parity)
            errorPos += pos;
    }

    // If an error is found, correct it
    if (errorPos) {
        printf("Error found at position: %d\n", errorPos);
        hamming[errorPos - 1] ^= 1;  // Flip the erroneous bit
        return 1; // Error corrected
    }
    return 0; // No error found
}

// Function to extract original data from Hamming Code
void extractOriginalData(int hamming[], int data[], int dataSize) {
    int parityCount = 0, i, j = 0;

    while ((1 << parityCount) < (dataSize + parityCount + 1)) {
        parityCount++;
    }

    int totalSize = dataSize + parityCount;

    for (i = 0; i < totalSize; i++) {
        if ((i + 1) & (i)) {  // Not a parity position
            data[j++] = hamming[i];
        }
    }
}

int main() {
    int sockfd, connfd;
    struct sockaddr_in servaddr, cliaddr;
    socklen_t len = sizeof(cliaddr);
    int hamming[10], data[DATA_BITS];

    // Creating socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    servaddr.sin_addr.s_addr = INADDR_ANY;

    // Bind and listen
    if (bind(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) != 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }
    if (listen(sockfd, 5) != 0) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    printf("Receiver waiting for connection...\n");
    connfd = accept(sockfd, (struct sockaddr*)&cliaddr, &len);
    if (connfd < 0) {
        perror("Server accept failed");
        exit(EXIT_FAILURE);
    }

    // Receive data
    recv(connfd, hamming, (DATA_BITS + 3) * sizeof(int), 0);
    printf("Received Hamming code data.\n");

    // Detect and correct error
    if (correctHammingCode(hamming, DATA_BITS)) {
        printf("Error corrected successfully.\n");
    } else {
        printf("No error found.\n");
    }

    // Extract original data
    extractOriginalData(hamming, data, DATA_BITS);

    // Print corrected data
    printf("Corrected Data: ");
    for (int i = 0; i < DATA_BITS; i++) {
        printf("%d ", data[i]);
    }
    printf("\n");

    close(connfd);
    close(sockfd);
    return 0;
}

/*
Receiver waiting for connection...
Received Hamming code data.
Error found at position: 2
Error corrected successfully.
Corrected Data: 1 0 1 1 
*/
