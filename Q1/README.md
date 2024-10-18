# Job Directory Blockchain

This program implements a simple job directory using blockchain principles. It allows users to add and search job listings while maintaining data integrity through cryptographic hashing and proof-of-work.

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
- A nonce for proof-of-work

The blockchain ensures data integrity by linking each block to the previous one through cryptographic hashes and implementing a proof-of-work system.

## Hashing Mechanism

The program uses SHA-256 hashing (from OpenSSL) to create a unique fingerprint for each block. This hash is calculated based on:
- The block's index
- The timestamp
- The job information
- The previous block's hash
- The nonce

This mechanism, combined with the proof-of-work system, ensures that any modification to a block will require significant computational effort, making tampering both detectable and difficult.

## Data Integrity

The program includes a `verify_integrity` function that checks:
1. If the proof-of-work for each block is valid
2. If the stored hash of each block matches its recalculated hash
3. If the `prev_hash` of each block matches the hash of the previous block

This allows the system to detect any unauthorized changes to the job listings.

## Persistence

The blockchain state is saved to a file (`blockchain.dat`) and loaded at the start of each session, ensuring that the job listings persist between program executions.

## Usage

The program presents a menu-driven interface with the following options:
1. Add Job
2. List Jobs
3. Search Jobs
4. Verify Integrity
5. Save Blockchain
6. Load Blockchain
7. Exit

Follow the on-screen prompts to interact with the job directory.
