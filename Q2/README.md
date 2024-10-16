# Supply Chain Blockchain System

This is a simple blockchain-based supply chain tracking system implemented in C. It demonstrates basic blockchain concepts such as blocks, transactions, mining, and cryptographic hashing, while following specific rules for blockchain operations.

## Features

1. Blockchain Crypto: SHA-256 hashing for block integrity
2. Blockchain Data Structures: Block and Blockchain structures with a pending block for transactions
3. Transactions: Structure with item ID, description, and a simple digital signature
4. Block Mining: Proof-of-work algorithm with specific rules for adding blocks
5. CLI: Menu-driven command-line interface for interacting with the blockchain

## Rules and Workflow

1. The blockchain must be initialized before any other operations can be performed.
2. Transactions are added to a pending block, not directly to the blockchain.
3. Mining a block is only possible when there's at least one transaction in the pending block.
4. Blocks can only be added to the blockchain through the mining process.

## Dependencies

This project requires the OpenSSL library for cryptographic functions. Make sure you have OpenSSL installed on your system.

## Compilation

To compile the program, use the following command:

```
gcc -o supply_chain_blockchain main.c -lssl -lcrypto
```

This will create an executable named `supply_chain_blockchain`.

## Running the Program

To run the program, execute:

```
./supply_chain_blockchain
```

## Using the Menu-Driven CLI

The program provides a menu-driven command-line interface with the following options:

1. Create new blockchain: Initialize a new blockchain (only available if not already initialized).
2. Add transaction: Add a new transaction to the pending block (only available after blockchain initialization).
3. Mine block: Mine the pending block and add it to the blockchain (only available when there are pending transactions).
4. Print blockchain: Display the entire blockchain (only available after blockchain initialization).
5. Exit: Exit the program.

Simply enter the number corresponding to your desired action when prompted. The available options will change based on the current state of the blockchain.

## Blockchain Implementation

This system implements a basic blockchain with the following characteristics:

- Each block contains transactions, a timestamp, the previous block's hash, and a nonce.
- Transactions include an item ID, description, and a simple digital signature.
- Block integrity is ensured through SHA-256 hashing.
- A proof-of-work algorithm is used for mining new blocks, requiring a specific number of leading zeros in the block hash.
- A pending block holds transactions until they are mined into a new block.
