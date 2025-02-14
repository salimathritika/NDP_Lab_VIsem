#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 8080
#define DATA_BITS 4  // Change this as needed

// Function to calculate Hamming code
void generateHammingCode(int data[], int hamming[], int dataSize) {
    int parityCount = 0, i, j, k = 0;
    
    // Calculate number of parity bits needed
    while ((1 << parityCount) < (dataSize + parityCount + 1)) {
        parityCount++;
    }

    int totalSize = dataSize + parityCount;

    // Initialize Hamming code array
    for (i = 0, j = 0; i < totalSize; i++) {
        if ((i + 1) & (i)) {  // If it's not a power of 2
            hamming[i] = data[j++];
        } else {
            hamming[i] = 0;  // Placeholder for parity bit
        }
    }

    // Calculate parity bits
    for (i = 0; i < parityCount; i++) {
        int pos = (1 << i);
        int parity = 0;
        for (j = pos - 1; j < totalSize; j += (pos * 2)) {
            for (k = 0; k < pos && (j + k) < totalSize; k++) {
                parity ^= hamming[j + k];
            }
        }
        hamming[pos - 1] = parity;
    }
}

// Function to introduce a single-bit error (for testing correction)
void introduceError(int hamming[], int size) {
    int errorPos = rand() % size;
    hamming[errorPos] ^= 1;  // Flip the bit
    printf("Error introduced at position: %d\n", errorPos + 1);
}

int main() {
    int sockfd;
    struct sockaddr_in servaddr;
    int data[DATA_BITS], hamming[10]; // Hamming size is dynamically calculated

    // Creating socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    servaddr.sin_addr.s_addr = INADDR_ANY;

    // Connect to receiver
    if (connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) != 0) {
        perror("Connection failed");
        exit(EXIT_FAILURE);
    }

    // Input 4-bit data
    printf("Enter %d-bit data (space separated): ", DATA_BITS);
    for (int i = 0; i < DATA_BITS; i++) {
        scanf("%d", &data[i]);
    }

    // Generate Hamming Code
    generateHammingCode(data, hamming, DATA_BITS);

    // Introduce an error (for testing)
    introduceError(hamming, DATA_BITS + 3);

    // Send data
    send(sockfd, hamming, (DATA_BITS + 3) * sizeof(int), 0);
    printf("Hamming encoded data sent.\n");

    close(sockfd);
    return 0;
}

/*
Enter 4-bit data (space separated): 1 0 1 1
Error introduced at position: 2
Hamming encoded data sent.
*/
