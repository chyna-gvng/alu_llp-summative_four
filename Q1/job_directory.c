#include "job_directory.h"

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

// Calculate the hash of a block
char* calculate_hash(Block* block) {
    static char hash[HASH_SIZE + 1];
    char buffer[1024];
    SHA256_CTX sha256;
    unsigned char hash_bytes[SHA256_DIGEST_LENGTH];
    
    // Concatenate block data into a single string
    snprintf(buffer, sizeof(buffer), "%d%ld%s%s%s%s%s%s%d",
             block->index, block->timestamp, block->job.id,
             block->job.title, block->job.company, block->job.location,
             block->job.description, block->prev_hash, block->nonce);
    
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

// Mine a block (find a hash with DIFFICULTY leading zeros)
void mine_block(Block* block) {
    char* hash;
    do {
        block->nonce++;
        hash = calculate_hash(block);
    } while (strncmp(hash, "0000", DIFFICULTY) != 0);
    
    strcpy(block->hash, hash);
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
    char prev_hash[HASH_SIZE + 1] = "N/A";  // Default for genesis block
    
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
    new_block->nonce = 0;
    
    // Generate job ID
    bc->job_count++;
    snprintf(job.id, sizeof(job.id), "J%04d", bc->job_count);
    
    new_block->job = job;
    strcpy(new_block->prev_hash, prev_hash);
    
    // Mine the block
    mine_block(new_block);
    
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
    
    if (current == NULL) {
        printf("No jobs available.\n");
        return;
    }
    
    while (current) {
        printf("Block %d\n", current->index);
        printf("Timestamp: %ld\n", current->timestamp);
        printf("Nonce: %d\n", current->nonce);
        printf("Job ID: %s\n", current->job.id);
        printf("Title: %s\n", current->job.title);
        printf("Company: %s\n", current->job.company);
        printf("Location: %s\n", current->job.location);
        printf("Description: %s\n", current->job.description);
        printf("Hash: %s\n", current->hash);
        printf("Previous Hash: %s\n", current->prev_hash);
        printf("\n");
        current = current->next;
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

// Verify the integrity of the blockchain
int verify_integrity(Blockchain* bc) {
    Block* current = bc->head;
    char calculated_hash[HASH_SIZE + 1];
    char prev_hash[HASH_SIZE + 1] = {0};
    
    while (current) {
        // Verify proof of work
        if (strncmp(current->hash, "0000", DIFFICULTY) != 0) {
            printf("Proof of work verification failed for block %d\n", current->index);
            return 0;
        }
        
        // Calculate and verify the hash of the current block
        strcpy(calculated_hash, calculate_hash(current));
        if (strcmp(calculated_hash, current->hash) != 0) {
            printf("Integrity breach detected at block %d\n", current->index);
            printf("Stored hash: %s\n", current->hash);
            printf("Calculated hash: %s\n", calculated_hash);
            return 0;
        }
        
        // Verify that the prev_hash matches the hash of the previous block
        if (current->index > 0 && strcmp(current->prev_hash, prev_hash) != 0) {
            printf("Integrity breach detected at block %d\n", current->index);
            printf("Stored previous hash: %s\n", current->prev_hash);
            printf("Actual previous hash: %s\n", prev_hash);
            return 0;
        }
        
        // Store the current hash as prev_hash for the next iteration
        strcpy(prev_hash, current->hash);
        current = current->next;
    }
    
    return 1;  // Integrity verified
}

// Save the blockchain to a file
int save_blockchain(Blockchain* bc, const char* filename) {
    FILE* file = fopen(filename, "wb");
    if (!file) {
        printf("Error opening file for writing\n");
        return 0;
    }
    
    Block* current = bc->head;
    while (current) {
        fwrite(current, sizeof(Block), 1, file);
        current = current->next;
    }
    
    fclose(file);
    return 1;
}

// Load the blockchain from a file
int load_blockchain(Blockchain* bc, const char* filename) {
    FILE* file = fopen(filename, "rb");
    if (!file) {
        printf("Error opening file for reading\n");
        return 0;
    }
    
    init_blockchain(bc);
    
    Block block;
    Block* prev = NULL;
    while (fread(&block, sizeof(Block), 1, file) == 1) {
        Block* new_block = (Block*)malloc(sizeof(Block));
        if (!new_block) {
            printf("Memory allocation failed\n");
            fclose(file);
            return 0;
        }
        
        *new_block = block;
        new_block->next = NULL;
        
        if (prev == NULL) {
            bc->head = new_block;
        } else {
            prev->next = new_block;
        }
        
        prev = new_block;
        bc->job_count++;
    }
    
    fclose(file);
    return 1;
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
        printf("Hash: %s\n", current->hash);
        printf("Nonce: %d\n\n", current->nonce);
        current = current->next;
    }
}
