#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int base;   // Base address of the segment in physical memory
    int limit;  // Limit (size) of the segment
} Segment;

// Function to convert logical address to physical address
int logicalToPhysical(int segmentNumber, int offset, Segment *segments, int numSegments) {
    if (segmentNumber < 0 || segmentNumber >= numSegments) {
        printf("Error: Invalid segment number %d\n", segmentNumber);
        return -1; // Invalid segment number
    }
    if (offset < 0 || offset >= segments[segmentNumber].limit) {
        printf("Error: Offset %d is out of bounds for segment %d\n", offset, segmentNumber);
        return -1; // Offset out of bounds
    }

    // Calculate the physical address
    return segments[segmentNumber].base + offset;
}

int main() {
    int numSegments;

    // Get number of segments from user
    printf("Enter the number of segments: ");
    scanf("%d", &numSegments);

    Segment *segments = (Segment *)malloc(numSegments * sizeof(Segment));

    // Input segment base addresses and limits
    for (int i = 0; i < numSegments; i++) {
        printf("Enter base and limit for segment %d: ", i);
        scanf("%d %d", &segments[i].base, &segments[i].limit);
    }

    // Logical address calculations
    int numAddresses;
    printf("Enter the number of logical addresses to convert: ");
    scanf("%d", &numAddresses);

    for (int i = 0; i < numAddresses; i++) {
        int segmentNumber, offset;
        printf("Enter segment number and offset for address %d: ", i + 1);
        scanf("%d %d", &segmentNumber, &offset);

        int physicalAddress = logicalToPhysical(segmentNumber, offset, segments, numSegments);

        if (physicalAddress != -1) {
            printf("Physical address for segment %d with offset %d: %d\n", segmentNumber, offset, physicalAddress);
        }
    }

    free(segments); // Free allocated memory
    return 0;
}


/*

./q3
Enter the number of segments: 5
Enter base and limit for segment 0: 1400 2400
Enter base and limit for segment 1: 6300
6700
Enter base and limit for segment 2: 4300 4700
Enter base and limit for segment 3: 3200 4300
Enter base and limit for segment 4: 4700 5700
Enter the number of logical addresses to convert: 3
Enter segment number and offset for address 1: 2 53
Physical address for segment 2 with offset 53: 4353  (4300 +53) 
Enter segment number and offset for address 2: 3 852
Physical address for segment 3 with offset 852: 4052
Enter segment number and offset for address 3: 0 1222
Physical address for segment 0 with offset 1222: 2622
 
 */
