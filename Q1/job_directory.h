#ifndef JOB_DIRECTORY_H
#define JOB_DIRECTORY_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <openssl/sha.h>
#include <ctype.h>

#define MAX_JOBS 100
#define HASH_SIZE 64
#define MAX_KEYWORD_LENGTH 50
#define DIFFICULTY 4  // Number of leading zeros required for proof of work

// Structure to represent a job listing
typedef struct {
    char id[6];             // Unique identifier for the job (J0001 to J9999+)
    char title[100];        // Job title
    char company[100];      // Company offering the job
    char location[100];     // Job location
    char description[500];  // Job description
} Job;

// Structure to represent a block in the blockchain
typedef struct Block {
    int index;              // Position of the block in the chain
    time_t timestamp;       // Time when the block was created
    Job job;                // Job data stored in this block
    char prev_hash[HASH_SIZE + 1];  // Hash of the previous block
    char hash[HASH_SIZE + 1];       // Hash of this block
    int nonce;              // Nonce for proof of work
    struct Block* next;     // Pointer to the next block
} Block;

// Structure to represent the blockchain
typedef struct {
    Block* head;            // Pointer to the first block in the chain
    int job_count;          // Counter for job IDs
} Blockchain;

// Function prototypes
void init_blockchain(Blockchain* bc);
void add_job(Blockchain* bc, Job job);
void list_jobs(Blockchain* bc);
void search_jobs(Blockchain* bc, const char* keyword);
int verify_integrity(Blockchain* bc);
void print_blockchain(Blockchain* bc);
char* calculate_hash(Block* block);
void to_lowercase(char *str);
int save_blockchain(Blockchain* bc, const char* filename);
int load_blockchain(Blockchain* bc, const char* filename);
void mine_block(Block* block);

#endif // JOB_DIRECTORY_H
