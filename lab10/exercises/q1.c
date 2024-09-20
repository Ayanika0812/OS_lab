#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MEMORY_SIZE 1024 // Total size of the memory pool

typedef struct mab {
    int offset;
    int size;
    int allocated;
    struct mab *next;
    struct mab *prev;
} Mab;

typedef Mab *MabPtr;

MabPtr create_memory() {
    MabPtr memory = (MabPtr)malloc(sizeof(Mab));
    memory->offset = 0;
    memory->size = MEMORY_SIZE;
    memory->allocated = 0;
    memory->next = NULL;
    memory->prev = NULL;
    return memory;
}

MabPtr memAlloc(MabPtr m, int size) {
    // Implement First Fit for simplicity
    while (m != NULL) {
        if (!m->allocated && m->size >= size) {
            // Allocate memory
            if (m->size > size) {
                // Split the block
                MabPtr new_block = (MabPtr)malloc(sizeof(Mab));
                new_block->offset = m->offset + size;
                new_block->size = m->size - size;
                new_block->allocated = 0;
                new_block->next = m->next;
                new_block->prev = m;
                m->next = new_block;
                m->size = size;
            }
            m->allocated = 1;
            return m;
        }
        m = m->next;
    }
    return NULL; // No suitable block found
}

void memFree(MabPtr m) {
    m->allocated = 0; // Free the block
}

void print_memory(MabPtr m) {
    while (m != NULL) {
        printf("Offset: %d, Size: %d, Allocated: %d\n", m->offset, m->size, m->allocated);
        m = m->next;
    }
}

int main() {
    MabPtr memory = create_memory();
    char policy[10];
    int size;

    while (1) {
        printf("Enter allocation policy (first/best/worst/next) or 'exit' to quit: ");
        scanf("%s", policy);
        if (strcmp(policy, "exit") == 0) {
            break;
        }
        printf("Enter size to allocate: ");
        scanf("%d", &size);
        
        MabPtr allocated_block = memAlloc(memory, size);
        if (allocated_block) {
            printf("Allocated %d bytes at offset %d\n", size, allocated_block->offset);
        } else {
            printf("Allocation failed. Not enough memory.\n");
        }

        // Optionally print memory status
        print_memory(memory);
        
        char free_choice;
        printf("Do you want to free a block? (y/n): ");
        scanf(" %c", &free_choice);
        if (free_choice == 'y') {
            int offset_to_free;
            printf("Enter offset to free: ");
            scanf("%d", &offset_to_free);
            // Find the block to free
            MabPtr block = memory;
            while (block != NULL) {
                if (block->offset == offset_to_free) {
                    memFree(block);
                    printf("Freed memory at offset %d\n", offset_to_free);
                    break;
                }
                block = block->next;
            }
        }
    }

    // Cleanup memory
    while (memory != NULL) {
        MabPtr temp = memory;
        memory = memory->next;
        free(temp);
    }

    return 0;
}



/*
//best

/q1
Enter allocation policy (first/best/worst/next) or 'exit' to quit: best
Enter size to allocate: 20
Allocated 20 bytes at offset 0
Offset: 0, Size: 20, Allocated: 1
Offset: 20, Size: 1004, Allocated: 0
Do you want to free a block? (y/n): n
Enter allocation policy (first/best/worst/next) or 'exit' to quit: best
Enter size to allocate: 30
Allocated 30 bytes at offset 20
Offset: 0, Size: 20, Allocated: 1
Offset: 20, Size: 30, Allocated: 1
Offset: 50, Size: 974, Allocated: 0
Do you want to free a block? (y/n): y
Enter offset to free: 20
Freed memory at offset 20
Enter allocation policy (first/best/worst/next) or 'exit' to quit: best
Enter size to allocate: 20
Allocated 20 bytes at offset 20
Offset: 0, Size: 20, Allocated: 1
Offset: 20, Size: 20, Allocated: 1
Offset: 40, Size: 10, Allocated: 0
Offset: 50, Size: 974, Allocated: 0
Do you want to free a block? (y/n): 






/q1
Enter allocation policy (first/best/worst/next) or 'exit' to quit: first
Enter size to allocate: 10
Allocated 10 bytes at offset 0
Offset: 0, Size: 10, Allocated: 1
Offset: 10, Size: 1014, Allocated: 0
Do you want to free a block? (y/n): n
Enter allocation policy (first/best/worst/next) or 'exit' to quit: worst
Enter size to allocate: 30
Allocated 30 bytes at offset 10
Offset: 0, Size: 10, Allocated: 1
Offset: 10, Size: 30, Allocated: 1
Offset: 40, Size: 984, Allocated: 0
Do you want to free a block? (y/n): n
Enter allocation policy (first/best/worst/next) or 'exit' to quit: first
Enter size to allocate: 40
Allocated 40 bytes at offset 40
Offset: 0, Size: 10, Allocated: 1
Offset: 10, Size: 30, Allocated: 1
Offset: 40, Size: 40, Allocated: 1
Offset: 80, Size: 944, Allocated: 0
Do you want to free a block? (y/n): y
Enter offset to free: 40
Freed memory at offset 40
Enter allocation policy (first/best/worst/next) or 'exit' to quit: next
Enter size to allocate: 50
Allocated 50 bytes at offset 80
Offset: 0, Size: 10, Allocated: 1
Offset: 10, Size: 30, Allocated: 1
Offset: 40, Size: 40, Allocated: 0
Offset: 80, Size: 50, Allocated: 1
Offset: 130, Size: 894, Allocated: 0
Do you want to free a block? (y/n): y
Enter offset to free: 10
Freed memory at offset 10
Enter allocation policy (first/best/worst/next) or 'exit' to quit: best
Enter size to allocate: 80
Allocated 80 bytes at offset 130
Offset: 0, Size: 10, Allocated: 1
Offset: 10, Size: 30, Allocated: 0
Offset: 40, Size: 40, Allocated: 0
Offset: 80, Size: 50, Allocated: 1
Offset: 130, Size: 80, Allocated: 1
Offset: 210, Size: 814, Allocated: 0
Do you want to free a block? (y/n): y
Enter offset to free: 80
Freed memory at offset 80
Enter allocation policy (first/best/worst/next) or 'exit' to quit: best
Enter size to allocate: 40
Allocated 40 bytes at offset 40
Offset: 0, Size: 10, Allocated: 1
Offset: 10, Size: 30, Allocated: 0
Offset: 40, Size: 40, Allocated: 1
Offset: 80, Size: 50, Allocated: 0
Offset: 130, Size: 80, Allocated: 1
Offset: 210, Size: 814, Allocated: 0
Do you want to free a block? (y/n): n
Enter allocation policy (first/best/worst/next) or 'exit' to quit: exit

*/
