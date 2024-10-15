#include "job_directory.h"
#include <ctype.h>

// Convert a string to lowercase
void to_lowercase(char *str) {
    for (int i = 0; str[i]; i++) {
        str[i] = tolower(str[i]);
    }
}

// Initialize the blockchain
void init_blockchain(Blockchain* bc) {
    bc->head = NULL;
    bc->job_count = 0;  // Initialize job count
}

// Add a new job to the blockchain
void add_job(Blockchain* bc, Job job) {
    Block* new_block = (Block*)malloc(sizeof(Block));
    if (!new_block) {
        printf("Memory allocation failed\n");
        return;
    }
    
    Block* last = bc->head;
    int index = 0;
    char prev_hash[HASH_SIZE + 1] = {0};
    
    // Find the last block and get its hash
    if (last) {
        while (last->next) {
            last = last->next;
        }
        index = last->index + 1;
        strcpy(prev_hash, last->hash);
    }
    
    new_block->index = index;
    new_block->timestamp = time(NULL);
    
    // Generate job ID
    bc->job_count++;
    snprintf(job.id, sizeof(job.id), "J%04d", bc->job_count);
    
    new_block->job = job;
    strcpy(new_block->prev_hash, prev_hash);
    
    // Calculate hash after setting all other fields
    strcpy(new_block->hash, calculate_hash(new_block));
    
    new_block->next = NULL;
    
    // Add the new block to the chain
    if (bc->head == NULL) {
        bc->head = new_block;
    } else {
        last->next = new_block;
    }
}

// List all jobs in the blockchain
void list_jobs(Blockchain* bc) {
    Block* current = bc->head;
    Block* blocks[MAX_JOBS];
    int count = 0;
    
    // Store blocks in an array
    while (current && count < MAX_JOBS) {
        blocks[count++] = current;
        current = current->next;
    }
    
    // Print blocks in reverse order (newest first)
    for (int i = count - 1; i >= 0; i--) {
        current = blocks[i];
        printf("Block %d\n", current->index);
        printf("Job ID: %s\n", current->job.id);
        printf("Title: %s\n", current->job.title);
        printf("Company: %s\n", current->job.company);
        printf("Location: %s\n", current->job.location);
        printf("Description: %s\n", current->job.description);
        printf("Current Hash: %s\n", current->hash);
        if (current->index > 0) {
            printf("Previous Hash: %s\n", current->prev_hash);
        } else {
            printf("Previous Hash: N/A\n");
        }
        printf("\n");
    }
    
    if (count == 0) {
        printf("No jobs available.\n");
    }
}

// Search for jobs using a keyword
void search_jobs(Blockchain* bc, const char* keyword) {
    Block* current = bc->head;
    int found = 0;
    char lower_keyword[MAX_KEYWORD_LENGTH];
    char lower_field[500];  // Assuming the longest field is the description

    // Convert keyword to lowercase for case-insensitive search
    strcpy(lower_keyword, keyword);
    to_lowercase(lower_keyword);
    
    while (current) {
        // Check each field for the keyword (case-insensitive)
        strcpy(lower_field, current->job.title);
        to_lowercase(lower_field);
        if (strstr(lower_field, lower_keyword)) {
            found = 1;
            goto print_job;
        }

        strcpy(lower_field, current->job.company);
        to_lowercase(lower_field);
        if (strstr(lower_field, lower_keyword)) {
            found = 1;
            goto print_job;
        }

        strcpy(lower_field, current->job.location);
        to_lowercase(lower_field);
        if (strstr(lower_field, lower_keyword)) {
            found = 1;
            goto print_job;
        }

        strcpy(lower_field, current->job.description);
        to_lowercase(lower_field);
        if (strstr(lower_field, lower_keyword)) {
            found = 1;
            goto print_job;
        }

        current = current->next;
        continue;

    print_job:
        printf("Job ID: %s\n", current->job.id);
        printf("Title: %s\n", current->job.title);
        printf("Company: %s\n", current->job.company);
        printf("Location: %s\n", current->job.location);
        printf("Description: %s\n\n", current->job.description);
        current = current->next;
    }
    
    if (!found) {
        printf("No jobs found matching the keyword: %s\n", keyword);
    }
}

// Modify an existing job in the blockchain
int modify_job(Blockchain* bc, const char* id, Job new_job) {
    Block* current = bc->head;
    
    while (current) {
        if (strcmp(current->job.id, id) == 0) {
            strcpy(new_job.id, current->job.id);  // Preserve the original ID
            current->job = new_job;
            strcpy(current->hash, calculate_hash(current));
            return 1;  // Job found and modified
        }
        current = current->next;
    }
    
    return 0;  // Job not found
}

// Delete a job from the blockchain
int delete_job(Blockchain* bc, const char* id) {
    Block* current = bc->head;
    Block* prev = NULL;
    
    while (current) {
        if (strcmp(current->job.id, id) == 0) {
            if (prev) {
                prev->next = current->next;
            } else {
                bc->head = current->next;
            }
            free(current);
            return 1;  // Job found and deleted
        }
        prev = current;
        current = current->next;
    }
    
    return 0;  // Job not found
}

// Verify the integrity of the blockchain
int verify_integrity(Blockchain* bc) {
    Block* current = bc->head;
    char calculated_hash[HASH_SIZE + 1];
    char prev_hash[HASH_SIZE + 1] = {0};
    
    while (current) {
        // Calculate and verify the hash of the current block
        strcpy(calculated_hash, calculate_hash(current));
        if (strcmp(calculated_hash, current->hash) != 0) {
            printf("Integrity breach detected at block %d\n", current->index);
            return 0;
        }
        
        // Verify that the prev_hash matches the hash of the previous block
        if (strcmp(current->prev_hash, prev_hash) != 0) {
            printf("Integrity breach detected at block %d\n", current->index);
            return 0;
        }
        
        // Store the current hash as prev_hash for the next iteration
        strcpy(prev_hash, current->hash);
        current = current->next;
    }
    
    return 1;  // Integrity verified
}

// Print the entire blockchain (for debugging purposes)
void print_blockchain(Blockchain* bc) {
    Block* current = bc->head;
    
    while (current) {
        printf("Block %d\n", current->index);
        printf("Timestamp: %ld\n", current->timestamp);
        printf("Job ID: %s\n", current->job.id);
        printf("Title: %s\n", current->job.title);
        printf("Company: %s\n", current->job.company);
        printf("Location: %s\n", current->job.location);
        printf("Description: %s\n", current->job.description);
        printf("Previous Hash: %s\n", current->prev_hash);
        printf("Hash: %s\n\n", current->hash);
        current = current->next;
    }
}

// Calculate the hash of a block
char* calculate_hash(Block* block) {
    static char hash[HASH_SIZE + 1];
    char buffer[1024];
    SHA256_CTX sha256;
    unsigned char hash_bytes[SHA256_DIGEST_LENGTH];
    
    // Concatenate block data into a single string
    snprintf(buffer, sizeof(buffer), "%d%ld%s%s%s%s%s%s",
             block->index, block->timestamp, block->job.id,
             block->job.title, block->job.company, block->job.location,
             block->job.description, block->prev_hash);
    
    // Calculate SHA256 hash
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, buffer, strlen(buffer));
    SHA256_Final(hash_bytes, &sha256);
    
    // Convert hash bytes to hexadecimal string
    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        sprintf(hash + (i * 2), "%02x", hash_bytes[i]);
    }
    
    return hash;
}
