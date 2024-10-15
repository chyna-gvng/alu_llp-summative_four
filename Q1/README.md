# Job Directory Blockchain

This program implements a simple job directory using blockchain principles. It allows users to add, search, modify, and delete job listings while maintaining data integrity through cryptographic hashing.

## Compilation and Execution

To compile the program, use the following command:

```
gcc -o job_directory main.c job_directory.c -lssl -lcrypto
```

To run the program:

```
./job_directory
```

To run the program and log all interactions(Linux):

```
script -c './job_directory' output.txt
```

## Blockchain Implementation

This program uses a linked list to represent the blockchain. Each block contains:
- Job information (ID, title, company, location, description)
- A timestamp
- The hash of the previous block
- The hash of the current block

The blockchain ensures data integrity by linking each block to the previous one through cryptographic hashes.

## Hashing Mechanism

The program uses SHA-256 hashing (from OpenSSL) to create a unique fingerprint for each block. This hash is calculated based on:
- The block's index
- The timestamp
- The job information
- The previous block's hash

This mechanism ensures that any modification to a block will change its hash and the hash of all subsequent blocks, making tampering easily detectable.

## Data Integrity

The program includes a `verify_integrity` function that checks:
1. If the stored hash of each block matches its recalculated hash
2. If the `prev_hash` of each block matches the hash of the previous block

This allows the system to detect any unauthorized changes to the job listings.