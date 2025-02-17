#include <stdio.h>
#include <stdlib.h>

#define BUCKET_SIZE 10  // The bucket size (in bytes)
#define OUTGOING_RATE 1 // Outgoing rate (1 byte per second)

void leakyBucket(int packetArrivals[], int n) {
    int bucket = 0; // Initial bucket size (empty)
    int currentTime = 0; // The current time for packet arrival
    int conformingPackets = 0;
    int nonConformingPackets = 0;

    printf("Packet Arrival Times (in seconds) with Size 4 Bytes Each:\n");
    printf("--------------------------------------------------------\n");

    // Process the packets
    for (int i = 0; i < n; i++) {
        // Get the packet arrival time
        currentTime = packetArrivals[i];

        // At each second, send out 1 byte if the bucket has any data
        if (bucket > 0) {
            bucket -= OUTGOING_RATE;
            printf("Time %d: Sent 1 byte from bucket. Bucket now has %d bytes.\n", currentTime, bucket);
        }

        // Add packet to bucket if there's space
        if (bucket + 4 <= BUCKET_SIZE) {
            bucket += 4; // Adding 4 bytes (size of each packet)
            conformingPackets++;
            printf("Time %d: Packet added to bucket. Bucket now has %d bytes.\n", currentTime, bucket);
        } else {
            // If the bucket overflows, reject the packet
            nonConformingPackets++;
            printf("Time %d: Packet rejected due to overflow. Bucket still has %d bytes.\n", currentTime, bucket);
        }
    }

    // Final output: conforming and non-conforming packets
    printf("\n-------------------------------------\n");
    printf("Total conforming packets: %d\n", conformingPackets);
    printf("Total non-conforming packets: %d\n", nonConformingPackets);
}

int main() {
    // Sample packet arrival times (in seconds)
    int packetArrivals[] = {1, 2, 3, 5, 6, 8, 11, 12, 15, 16, 19};
    int n = sizeof(packetArrivals) / sizeof(packetArrivals[0]);

    // Call the leaky bucket function to process the packets
    leakyBucket(packetArrivals, n);
    

    return 0;
}

/*
Packet Arrival Times (in seconds) with Size 4 Bytes Each:
--------------------------------------------------------
Time 1: Packet added to bucket. Bucket now has 4 bytes.
Time 2: Sent 1 byte from bucket. Bucket now has 3 bytes.
Time 2: Packet added to bucket. Bucket now has 7 bytes.
Time 3: Sent 1 byte from bucket. Bucket now has 6 bytes.
Time 3: Packet added to bucket. Bucket now has 10 bytes.
Time 5: Sent 1 byte from bucket. Bucket now has 9 bytes.
Time 5: Packet rejected due to overflow. Bucket still has 9 bytes.
Time 6: Sent 1 byte from bucket. Bucket now has 8 bytes.
Time 6: Packet rejected due to overflow. Bucket still has 8 bytes.
Time 8: Sent 1 byte from bucket. Bucket now has 7 bytes.
Time 8: Packet rejected due to overflow. Bucket still has 7 bytes.
Time 11: Sent 1 byte from bucket. Bucket now has 6 bytes.
Time 11: Packet added to bucket. Bucket now has 10 bytes.
Time 12: Sent 1 byte from bucket. Bucket now has 9 bytes.
Time 12: Packet rejected due to overflow. Bucket still has 9 bytes.
Time 15: Sent 1 byte from bucket. Bucket now has 8 bytes.
Time 15: Packet rejected due to overflow. Bucket still has 8 bytes.
Time 16: Sent 1 byte from bucket. Bucket now has 7 bytes.
Time 16: Packet rejected due to overflow. Bucket still has 7 bytes.
Time 19: Sent 1 byte from bucket. Bucket now has 6 bytes.
Time 19: Packet added to bucket. Bucket now has 10 bytes.

-------------------------------------
Total conforming packets: 5
Total non-conforming packets: 6
*/
