#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 8080
#define POLY_CRC12  0x80F
#define POLY_CRC16  0x8005
#define POLY_CCITT  0x1021

// Function to compute CRC
unsigned int compute_crc(const char *data, int len, unsigned int poly, int bits) {
    unsigned int crc = 0;
    for (int i = 0; i < len; i++) {
        crc ^= (data[i] << (bits - 8));
        for (int j = 0; j < 8; j++) {
            if (crc & (1 << (bits - 1)))
                crc = (crc << 1) ^ poly;
            else
                crc <<= 1;
        }
    }
    return crc & ((1 << bits) - 1);
}

int main() {
    int sockfd, connfd;
    struct sockaddr_in servaddr, cliaddr;
    socklen_t len = sizeof(cliaddr);
    char buffer[1050];

    // Create socket
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

    // Receive message
    recv(connfd, buffer, sizeof(buffer), 0);
    buffer[strcspn(buffer, "\n")] = '\0';  // Remove trailing newline

    // Parse received data
    char message[1024];
    unsigned int received_crc12, received_crc16, received_crcccitt;
    sscanf(buffer, "%[^|]|%x|%x|%x", message, &received_crc12, &received_crc16, &received_crcccitt);

    // Compute CRCs
    unsigned int calc_crc12 = compute_crc(message, strlen(message), POLY_CRC12, 12);
    unsigned int calc_crc16 = compute_crc(message, strlen(message), POLY_CRC16, 16);
    unsigned int calc_crcccitt = compute_crc(message, strlen(message), POLY_CCITT, 16);

    // Check if CRCs match
    printf("Received message: %s\n", message);
    printf("Checking CRCs...\n");

    if (calc_crc12 == received_crc12)
        printf("CRC-12: OK\n");
    else
        printf("CRC-12: ERROR (Expected: %03X, Received: %03X)\n", calc_crc12, received_crc12);

    if (calc_crc16 == received_crc16)
        printf("CRC-16: OK\n");
    else
        printf("CRC-16: ERROR (Expected: %04X, Received: %04X)\n", calc_crc16, received_crc16);

    if (calc_crcccitt == received_crcccitt)
        printf("CRC-CCITT: OK\n");
    else
        printf("CRC-CCITT: ERROR (Expected: %04X, Received: %04X)\n", calc_crcccitt, received_crcccitt);

    close(connfd);
    close(sockfd);
    return 0;
}

/*
Receiver waiting for connection...
Received message: hello
Checking CRCs...
CRC-12: OK
CRC-16: OK
CRC-CCITT: OK
*/
