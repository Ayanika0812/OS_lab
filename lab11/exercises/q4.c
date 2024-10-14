#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_BLOCKS 100
#define SPARE_BLOCKS 10

// Structure to represent the bad block table
struct BadBlock {
    int bad_block_address;
    int spare_block_address;
};

// Array to hold the bad block table
struct BadBlock bad_blocks[MAX_BLOCKS];
int bad_block_count = 0;

// Function to find a spare block
int findSpareBlock(bool *disk) {
    for (int i = 0; i < MAX_BLOCKS; i++) {
        if (disk[i] == false) {
            disk[i] = true; // Mark as allocated
            return i; // Return the spare block address
        }
    }
    return -1; // No spare block available
}

// Function to mark a block as bad and allocate a spare block
void markBadBlock(bool *disk, int bad_block) {
    if (bad_block_count >= SPARE_BLOCKS) {
        printf("Error: No more spare blocks available.\n");
        return;
    }

    // Find a spare block
    int spare_block = findSpareBlock(disk);
    if (spare_block == -1) {
        printf("Error: No spare block available to allocate for bad block %d.\n", bad_block);
        return;
    }

    // Update the bad blocks table
    bad_blocks[bad_block_count].bad_block_address = bad_block;
    bad_blocks[bad_block_count].spare_block_address = spare_block;
    bad_block_count++;

    printf("Bad block %d marked. Spare block allocated at %d.\n", bad_block, spare_block);
}

// Function to simulate disk access
void accessDiskBlock(bool *disk, int block) {
    // Check if the block is bad and consult the bad blocks table
    for (int i = 0; i < bad_block_count; i++) {
        if (bad_blocks[i].bad_block_address == block) {
            printf("Accessing bad block %d. Redirecting to spare block %d.\n",
                   block, bad_blocks[i].spare_block_address);
            return;
        }
    }

    // Check if the block is allocated
    if (disk[block]) {
        printf("Accessing block %d successfully.\n", block);
    } else {
        printf("Error: Block %d is not allocated.\n", block);
    }
}

int main() {
    bool disk[MAX_BLOCKS] = {false}; // False indicates unallocated block
    int choice, block;

    // Pre-allocate some blocks (for demonstration)
    for (int i = 0; i < 50; i++) {
        disk[i] = true; // Mark the first 50 blocks as allocated
    }

    while (1) {
        printf("\nFile System Simulation Menu:\n");
        printf("1. Mark Bad Block\n");
        printf("2. Access Disk Block\n");
        printf("3. Exit\n");
        printf("Choose an option: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("Enter the bad block address (0 to %d): ", MAX_BLOCKS - 1);
                scanf("%d", &block);
                if (block < 0 || block >= MAX_BLOCKS) {
                    printf("Invalid block address.\n");
                } else {
                    markBadBlock(disk, block);
                }
                break;
            case 2:
                printf("Enter the block address to access (0 to %d): ", MAX_BLOCKS - 1);
                scanf("%d", &block);
                if (block < 0 || block >= MAX_BLOCKS) {
                    printf("Invalid block address.\n");
                } else {
                    accessDiskBlock(disk, block);
                }
                break;
            case 3:
                exit(0);
            default:
                printf("Invalid option. Please try again.\n");
        }
    }

    return 0;
}

/*
./a.out

File System Simulation Menu:
1. Mark Bad Block
2. Access Disk Block
3. Exit
Choose an option: 2
Enter the block address to access (0 to 99): 1
Accessing block 1 successfully.

File System Simulation Menu:
1. Mark Bad Block
2. Access Disk Block
3. Exit
Choose an option: 1
Enter the bad block address (0 to 99): 10
Bad block 10 marked. Spare block allocated at 50.

File System Simulation Menu:
1. Mark Bad Block
2. Access Disk Block
3. Exit
Choose an option: 2
Enter the block address to access (0 to 99): 10
Accessing bad block 10. Redirecting to spare block 50.

File System Simulation Menu:
1. Mark Bad Block
2. Access Disk Block
3. Exit
Choose an option: 1
Enter the bad block address (0 to 99): 60
Bad block 60 marked. Spare block allocated at 51.

File System Simulation Menu:
1. Mark Bad Block
2. Access Disk Block
3. Exit
Choose an option: 2
Enter the block address to access (0 to 99): 4
Accessing block 4 successfully.

File System Simulation Menu:
1. Mark Bad Block
2. Access Disk Block
3. Exit
Choose an option: 2
Enter the block address to access (0 to 99): 60
Accessing bad block 60. Redirecting to spare block 51.

File System Simulation Menu:
1. Mark Bad Block
2. Access Disk Block
3. Exit
Choose an option: 2
Enter the block address to access (0 to 99): 51
Accessing block 51 successfully.

File System Simulation Menu:
1. Mark Bad Block
2. Access Disk Block
3. Exit
Choose an option: 3
*/
