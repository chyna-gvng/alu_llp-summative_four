#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <openssl/sha.h>
#include <stdbool.h>

#define MAX_TRANSACTIONS 10
#define DIFFICULTY 4 // Number of leading zeros required in hash

// Transaction structure
typedef struct {
    int item_id;
    char description[256];
    char signature[65]; // SHA-256 produces a 64-character hex string
} Transaction;

// Block structure
typedef struct Block {
    int index;
    time_t timestamp;
    Transaction transactions[MAX_TRANSACTIONS];
    int transaction_count;
    char previous_hash[65];
    char hash[65];
    int nonce;
    struct Block* next;
} Block;

// Blockchain structure
typedef struct {
    Block* head;
    Block* pending_block; // Block to store pending transactions
    bool is_initialized;
} Blockchain;

// Function prototypes
void calculate_hash(Block* block, char* hash);
void mine_block(Block* block);
bool add_transaction(Block* block, int item_id, const char* description);
void create_blockchain(Blockchain* blockchain);
void add_block(Blockchain* blockchain, Block* new_block);
void print_blockchain(Blockchain* blockchain);
void clear_input_buffer();
int display_menu(bool blockchain_initialized, bool has_pending_transactions);

// Calculate SHA-256 hash of a block
void calculate_hash(Block* block, char* hash) {
    char input[1024];
    snprintf(input, sizeof(input), "%d%ld%d%s%d", 
             block->index, block->timestamp, block->transaction_count, 
             block->previous_hash, block->nonce);

    unsigned char hash_bytes[SHA256_DIGEST_LENGTH];
    SHA256((unsigned char*)input, strlen(input), hash_bytes);

    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        sprintf(&hash[i * 2], "%02x", hash_bytes[i]);
    }
}

// Mine a block (find a nonce that produces a hash with DIFFICULTY leading zeros)
void mine_block(Block* block) {
    char hash[65];
    do {
        block->nonce++;
        calculate_hash(block, hash);
    } while (strncmp(hash, "0000", DIFFICULTY) != 0);

    strcpy(block->hash, hash);
}

// Add a transaction to a block
bool add_transaction(Block* block, int item_id, const char* description) {
    if (block->transaction_count >= MAX_TRANSACTIONS) {
        return false; // Block is full
    }

    Transaction* t = &block->transactions[block->transaction_count++];
    t->item_id = item_id;
    strncpy(t->description, description, sizeof(t->description) - 1);
    
    // Generate a simple "signature" (in a real system, this would be more complex)
    char signature_input[512];
    snprintf(signature_input, sizeof(signature_input), "%d%s", item_id, description);
    unsigned char signature_bytes[SHA256_DIGEST_LENGTH];
    SHA256((unsigned char*)signature_input, strlen(signature_input), signature_bytes);
    
    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        sprintf(&t->signature[i * 2], "%02x", signature_bytes[i]);
    }

    return true;
}

// Create a new blockchain
void create_blockchain(Blockchain* blockchain) {
    blockchain->head = NULL;
    blockchain->is_initialized = true;

    // Create a new pending block
    blockchain->pending_block = (Block*)malloc(sizeof(Block));
    if (blockchain->pending_block == NULL) {
        fprintf(stderr, "Error: Unable to allocate memory for pending block.\n");
        exit(1);
    }
    blockchain->pending_block->index = 0;
    blockchain->pending_block->transaction_count = 0;
    strcpy(blockchain->pending_block->previous_hash, "0"); // Genesis block
}

// Add a new block to the blockchain
void add_block(Blockchain* blockchain, Block* new_block) {
    new_block->timestamp = time(NULL);
    new_block->next = blockchain->head;
    
    if (blockchain->head != NULL) {
        strcpy(new_block->previous_hash, blockchain->head->hash);
        new_block->index = blockchain->head->index + 1;
    }

    mine_block(new_block);
    blockchain->head = new_block;
}

// Print the entire blockchain
void print_blockchain(Blockchain* blockchain) {
    if (!blockchain->is_initialized) {
        printf("Blockchain has not been initialized yet.\n");
        return;
    }

    if (blockchain->head == NULL) {
        printf("Blockchain is empty. No blocks have been mined yet.\n");
        return;
    }

    Block* current = blockchain->head;
    while (current != NULL) {
        printf("Block %d\n", current->index);
        printf("Timestamp: %ld\n", current->timestamp);
        printf("Transactions:\n");
        for (int i = 0; i < current->transaction_count; i++) {
            printf("  Item ID: %d, Description: %s\n", 
                   current->transactions[i].item_id, 
                   current->transactions[i].description);
        }
        printf("Previous Hash: %s\n", current->previous_hash);
        printf("Hash: %s\n", current->hash);
        printf("Nonce: %d\n\n", current->nonce);
        current = current->next;
    }
}

// Function to clear the input buffer
void clear_input_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) { }
}

// Function to display the menu and get user choice
int display_menu(bool blockchain_initialized, bool has_pending_transactions) {
    int choice;
    printf("\n--- Supply Chain Blockchain System ---\n");
    printf("1. Create new blockchain\n");
    if (blockchain_initialized) {
        printf("2. Add transaction\n");
        if (has_pending_transactions) {
            printf("3. Mine block\n");
        }
        printf("4. Print blockchain\n");
    }
    printf("5. Exit\n");
    printf("Enter your choice: ");
    scanf("%d", &choice);
    clear_input_buffer();
    return choice;
}

// Main function with menu-driven CLI
int main() {
    Blockchain blockchain = {NULL, NULL, false};
    int choice;
    
    while (1) {
        choice = display_menu(blockchain.is_initialized, 
                              blockchain.pending_block && blockchain.pending_block->transaction_count > 0);
        
        switch (choice) {
            case 1:
                if (blockchain.is_initialized) {
                    printf("Blockchain is already initialized.\n");
                } else {
                    create_blockchain(&blockchain);
                    printf("New blockchain created.\n");
                }
                break;
            
            case 2:
                if (!blockchain.is_initialized) {
                    printf("Please initialize the blockchain first.\n");
                    break;
                }
                int item_id;
                char description[256];
                printf("Enter item ID: ");
                scanf("%d", &item_id);
                clear_input_buffer();
                printf("Enter item description: ");
                fgets(description, sizeof(description), stdin);
                description[strcspn(description, "\n")] = 0; // Remove trailing newline
                
                if (add_transaction(blockchain.pending_block, item_id, description)) {
                    printf("Transaction added to pending block.\n");
                } else {
                    printf("Failed to add transaction. Pending block might be full.\n");
                }
                break;
            
            case 3:
                if (!blockchain.is_initialized) {
                    printf("Please initialize the blockchain first.\n");
                    break;
                }
                if (!blockchain.pending_block || blockchain.pending_block->transaction_count == 0) {
                    printf("No pending transactions to mine.\n");
                    break;
                }
                add_block(&blockchain, blockchain.pending_block);
                printf("New block mined and added to the blockchain.\n");
                
                // Create a new pending block
                blockchain.pending_block = (Block*)malloc(sizeof(Block));
                if (blockchain.pending_block == NULL) {
                    fprintf(stderr, "Error: Unable to allocate memory for new pending block.\n");
                    exit(1);
                }
                blockchain.pending_block->transaction_count = 0;
                break;
            
            case 4:
                if (!blockchain.is_initialized) {
                    printf("Please initialize the blockchain first.\n");
                    break;
                }
                print_blockchain(&blockchain);
                break;
            
            case 5:
                printf("Exiting program. Goodbye!\n");
                exit(0);
            
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }

    return 0;
}