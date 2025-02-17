#include <stdio.h>
#include <stdlib.h>

#define REPLENISH_RATE 10  // tokens per second (10 KBps)
#define MAX_BUCKET_SIZE 50  // Maximum bucket size in KB
#define PACKET_SIZE 15  // Size of packet in KB
#define SENDER_INTERVAL 0.5  // Interval in seconds between packet sends

void tokenBucketSimulation(double totalTime) {
    double tokens = MAX_BUCKET_SIZE;  // The bucket starts full
    double timeElapsed = 0.0;  // Time elapsed in seconds
    double queuedPackets = 0;  // Number of queued packets (in KB)

    printf("Token Bucket Simulation\n");
    printf("------------------------\n");

    while (timeElapsed < totalTime) {
        // Tokens are added at the replenish rate per second
        tokens += REPLENISH_RATE * SENDER_INTERVAL;  // Add tokens based on interval (in KB)
        if (tokens > MAX_BUCKET_SIZE) {
            tokens = MAX_BUCKET_SIZE;  // Prevent overflow, cap at bucket size
        }

        // Sender attempts to send a packet
        if (tokens >= PACKET_SIZE) {
            tokens -= PACKET_SIZE;  // Remove tokens for the packet
            printf("At time %.1f seconds, packet sent (15 KB), tokens left: %.1f KB\n", timeElapsed, tokens);
        } else {
            // Not enough tokens to send packet
            queuedPackets += PACKET_SIZE;  // Queue the packet
            printf("At time %.1f seconds, packet queued (15 KB), tokens left: %.1f KB\n", timeElapsed, tokens);
        }

        timeElapsed += SENDER_INTERVAL;  // Move to the next time interval
    }

    printf("\nFinal status:\n");
    printf("Tokens left in bucket after %.1f seconds: %.1f KB\n", totalTime, tokens);
    printf("Total queued packets: %.1f KB\n", queuedPackets);
}

int main() {
    // i. How many tokens are left in the bucket after 1.5 seconds?
    printf("1. Tokens left after 1.5 seconds:\n");
    tokenBucketSimulation(1.5);

    // ii. How long will it take until packets start to be queued or dropped?
    printf("\n2. Time until packets start being queued or dropped:\n");
    double time = 0;
    double tokens = MAX_BUCKET_SIZE;
    while (time < 10) {  // Start from t=0 and check how long until packets start queuing
        tokens += REPLENISH_RATE * SENDER_INTERVAL;
        if (tokens > MAX_BUCKET_SIZE) tokens = MAX_BUCKET_SIZE;  // Cap at bucket size
        if (tokens < PACKET_SIZE) {
            printf("Time when packets start being queued: %.1f seconds\n", time);
            break;
        }
        tokens -= PACKET_SIZE;  // Send packet
        time += SENDER_INTERVAL;  // Move to next time interval
    }

    // iii. Calculate the maximum possible burst size with a max rate of 20 KBps
    printf("\n3. Maximum possible burst size with max rate 20 KBps:\n");
    double maxRate = 20;  // Maximum rate (in KBps)
    double maxBurstSize = MAX_BUCKET_SIZE + (maxRate * 1);  // Burst size is bucket size + tokens added in 1 second
    printf("Maximum possible burst size: %.1f KB\n", maxBurstSize);

    return 0;
}

/*
1. Tokens left after 1.5 seconds:
Token Bucket Simulation
------------------------
At time 0.0 seconds, packet sent (15 KB), tokens left: 35.0 KB
At time 0.5 seconds, packet sent (15 KB), tokens left: 25.0 KB
At time 1.0 seconds, packet sent (15 KB), tokens left: 15.0 KB

Final status:
Tokens left in bucket after 1.5 seconds: 15.0 KB
Total queued packets: 0.0 KB

2. Time until packets start being queued or dropped:
Time when packets start being queued: 2.0 seconds

3. Maximum possible burst size with max rate 20 KBps:
Maximum possible burst size: 70.0 KB
*/
