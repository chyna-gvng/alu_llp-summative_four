#include "job_directory.h"

// Function to print the menu options
void print_menu() {
    printf("\nJob Directory Blockchain Menu:\n");
    printf("1. Add Job\n");
    printf("2. List Jobs\n");
    printf("3. Search Jobs\n");
    printf("4. Modify Job\n");
    printf("5. Delete Job\n");
    printf("6. Verify Integrity\n");
    printf("7. Exit\n");
    printf("Enter your choice: ");
}

// Function to input job details from the user
Job input_job() {
    Job job;
    printf("Enter Job Title: ");
    fgets(job.title, sizeof(job.title), stdin);
    job.title[strcspn(job.title, "\n")] = 0; // Remove newline
    printf("Enter Company: ");
    fgets(job.company, sizeof(job.company), stdin);
    job.company[strcspn(job.company, "\n")] = 0;
    printf("Enter Location: ");
    fgets(job.location, sizeof(job.location), stdin);
    job.location[strcspn(job.location, "\n")] = 0;
    printf("Enter Job Description: ");
    fgets(job.description, sizeof(job.description), stdin);
    job.description[strcspn(job.description, "\n")] = 0;
    return job;
}

int main() {
    Blockchain bc;
    init_blockchain(&bc);
    int choice;
    char id[6];
    char keyword[MAX_KEYWORD_LENGTH];
    Job job;

    while (1) {
        print_menu();
        scanf("%d", &choice);
        getchar(); // Consume newline

        switch (choice) {
            case 1: // Add Job
                job = input_job();
                add_job(&bc, job);
                printf("Job added successfully with ID: %s\n", bc.head->job.id);
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
            case 4: // Modify Job
                printf("Enter Job ID to modify: ");
                fgets(id, sizeof(id), stdin);
                id[strcspn(id, "\n")] = 0; // Remove newline
                job = input_job();
                if (modify_job(&bc, id, job)) {
                    printf("Job modified successfully.\n");
                } else {
                    printf("Job not found.\n");
                }
                break;
            case 5: // Delete Job
                printf("Enter Job ID to delete: ");
                fgets(id, sizeof(id), stdin);
                id[strcspn(id, "\n")] = 0; // Remove newline
                if (delete_job(&bc, id)) {
                    printf("Job deleted successfully.\n");
                } else {
                    printf("Job not found.\n");
                }
                break;
            case 6: // Verify Integrity
                if (verify_integrity(&bc)) {
                    printf("Blockchain integrity verified.\n");
                } else {
                    printf("Blockchain integrity compromised.\n");
                }
                break;
            case 7: // Exit
                printf("Exiting program.\n");
                return 0;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }

    return 0;
}