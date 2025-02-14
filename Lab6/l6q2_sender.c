#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 8080
#define POLY_CRC12  0x80F  // CRC-12 polynomial
#define POLY_CRC16  0x8005 // CRC-16 polynomial
#define POLY_CCITT  0x1021 // CRC-CCITT polynomial

// Function to compute CRC using bitwise method
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
    return crc & ((1 << bits) - 1);  // Mask to appropriate bit size
}

int main() {
    int sockfd;
    struct sockaddr_in servaddr;
    char message[1024];

    // Create socket
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

    printf("Enter message to send: ");
    fgets(message, sizeof(message), stdin);
    message[strcspn(message, "\n")] = '\0';  // Remove newline character

    // Compute CRCs
    unsigned int crc12 = compute_crc(message, strlen(message), POLY_CRC12, 12);
    unsigned int crc16 = compute_crc(message, strlen(message), POLY_CRC16, 16);
    unsigned int crcccitt = compute_crc(message, strlen(message), POLY_CCITT, 16);

    // Send message along with CRCs
    char buffer[1050];
    snprintf(buffer, sizeof(buffer), "%s|%04X|%04X|%04X", message, crc12, crc16, crcccitt);

    send(sockfd, buffer, strlen(buffer), 0);
    printf("Message sent with CRCs.\n");

    close(sockfd);
    return 0;
}

/*
Enter message to send: hello
Message sent with CRCs.
*/
