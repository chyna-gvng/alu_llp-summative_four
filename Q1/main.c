#include "job_directory.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BLOCKCHAIN_FILE "blockchain.dat"

// Function to print the menu options
void print_menu() {
    printf("\nJob Directory Blockchain Menu:\n");
    printf("1. Add Job\n");
    printf("2. List Jobs\n");
    printf("3. Search Jobs\n");
    printf("4. Verify Integrity\n");
    printf("5. Save Blockchain\n");
    printf("6. Load Blockchain\n");
    printf("7. Exit\n");
    printf("Enter your choice: ");
}

// Function to clear the input buffer
void flush_input() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// Function to input job details from the user
Job input_job() {
    Job job;
    
    printf("Enter Job Title: ");
    fgets(job.title, sizeof(job.title), stdin);
    job.title[strcspn(job.title, "\n")] = 0;  // Remove newline character
    
    printf("Enter Company: ");
    fgets(job.company, sizeof(job.company), stdin);
    job.company[strcspn(job.company, "\n")] = 0;  // Remove newline character
    
    printf("Enter Location: ");
    fgets(job.location, sizeof(job.location), stdin);
    job.location[strcspn(job.location, "\n")] = 0;  // Remove newline character
    
    printf("Enter Job Description: ");
    fgets(job.description, sizeof(job.description), stdin);
    job.description[strcspn(job.description, "\n")] = 0;  // Remove newline character
    
    return job;
}

int main() {
    Blockchain bc;
    init_blockchain(&bc);
    int choice;
    char keyword[MAX_KEYWORD_LENGTH];
    Job job;

    // Load existing blockchain if file exists
    if (load_blockchain(&bc, BLOCKCHAIN_FILE)) {
        printf("Existing blockchain loaded.\n");
    } else {
        printf("No existing blockchain found. Starting with an empty chain.\n");
    }

    while (1) {
        print_menu();
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input. Please enter a number.\n");
            flush_input();
            continue;
        }
        flush_input();  // Clear any remaining input

        switch (choice) {
            case 1: // Add Job
                job = input_job();
                add_job(&bc, job);
                printf("Job added successfully.\n");
                break;
            case 2: // List Jobs
                list_jobs(&bc);
                break;
            case 3: // Search Jobs
                printf("Enter search keyword: ");
                fgets(keyword, MAX_KEYWORD_LENGTH, stdin);
                keyword[strcspn(keyword, "\n")] = 0; // Remove newline
                search_jobs(&bc, keyword);
                break;
            case 4: // Verify Integrity
                if (verify_integrity(&bc)) {
                    printf("Blockchain integrity verified.\n");
                } else {
                    printf("Blockchain integrity compromised.\n");
                }
                break;
            case 5: // Save Blockchain
                if (save_blockchain(&bc, BLOCKCHAIN_FILE)) {
                    printf("Blockchain saved successfully.\n");
                } else {
                    printf("Failed to save blockchain.\n");
                }
                break;
            case 6: // Load Blockchain
                if (load_blockchain(&bc, BLOCKCHAIN_FILE)) {
                    printf("Blockchain loaded successfully.\n");
                } else {
                    printf("Failed to load blockchain.\n");
                }
                break;
            case 7: // Exit
                printf("Saving blockchain before exiting...\n");
                if (save_blockchain(&bc, BLOCKCHAIN_FILE)) {
                    printf("Blockchain saved successfully.\n");
                } else {
                    printf("Failed to save blockchain.\n");
                }
                printf("Exiting program.\n");
                return 0;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }

    return 0;
}
