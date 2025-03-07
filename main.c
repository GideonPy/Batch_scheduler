#include "AUbatch.h"
#include <ctype.h>

void display_menu() {
    printf("\n===================== AUbatch Scheduler Menu =====================\n");
    printf("\nAvailable Commands:\n");
    printf(" submit - Submit jobs and choose scheduling policy before execution.\n");
    printf(" start - Begin job execution from Scheduled Queue.\n");
    printf(" list-submitted - List jobs in Submission Queue.\n");
    printf(" list-scheduled - List jobs in Scheduled Queue.\n");
    printf(" quit - Exit the program without running performance metrics.\n");
    printf("====================================================================\n");
}



int main() {
    char command[256];

    // Initialize scheduler
    initialize_scheduler();
    
    // Prompt user to submit jobs at startup
    initialize_job_submission();
    
    // Display the Main menu
    display_menu();

    //---------------------
    // MAIN COMMAND LOOP
    //--------------------
    while (1) {
        printf("\n> ");
	if (!fgets(command, sizeof(command), stdin)) {
	  error_message("Failed to read command. Try again.");
	  continue;
	}
	// Remove trailing newline
        command[strcspn(command, "\n")] = 0;

	//-----------------
	// SUBMIT Commmand
	//-----------------
        if (strncmp(command, "submit", 6) == 0) {
            int policy;
            printf("Choose a scheduling policy before execution:\n");
            printf("0 - FCFS | 1 - SJF | 2 - Priority\n> ");
	    if (scanf("%d", &policy) != 1) {
	      error_message("Invalid input. Please enter 0,1, or 2.");
	      // Clear leftover input
	      while (getchar() != '\n');
	      continue;
	    }
	    // Consume leftover newline
	    while (getchar() != '\n')

            if (policy < 0 || policy > 2) {
                error_message("Invalid policy selection.");
                continue;
            }

            apply_scheduling_policy(policy);
            printf("Scheduling policy set to: %d\n", policy);
            printf("\nType 'start' to begin execution\n");
        }

	//-----------------
	// START Command
	//-----------------
	
        else if (strcmp(command, "start") == 0) {
	  pthread_mutex_lock(&job_queue_mutex);
	  // If no jobs in scheduled_queue_head and no jobs in submission_queue_head
            if (!scheduled_queue_head && submission_queue_head) {
	      pthread_mutex_unlock(&job_queue_mutex);
                error_message("No jobs available in Scheduled Queue or Submission Queue.");
                continue;
            }

	    pthread_mutex_unlock(&job_queue_mutex);
	    
	    printf("Starting job execution...\n");
	    
	    // Signal dispatcher jobs are ready
	     pthread_cond_signal(&job_available);
	    
	     // Wait until dispather empties
	    pthread_mutex_lock(&job_queue_mutex); 
	    while (scheduled_queue_head) {
	      pthread_mutex_unlock(&job_queue_mutex);
	      sleep(1); // Allow execution to proceed
	      pthread_mutex_lock(&job_queue_mutex);
	    }
	    pthread_mutex_unlock(&job_queue_mutex);

	    // Flush any "Job X completed." lines from dispatcher
	    flush_completion_messages();
	    
	    // Print performance after execution
	    print_performance_metrics();
	    
	    // Ask for rerun with different policy.
	    printf("\nWould you like to change the scheduling policy and rerun the queue? (yes/no)\n> ");
	    char choice[32];
	    if (!fgets(choice, sizeof(choice), stdin)) {
	      error_message("Invalid input. Aborting re-run prompt");
	      continue;
	    }
	    
	    // Remove trailing newline
	    choice[strcspn(choice, "\n")] = '\0';

	    // Comvert user input to lower case
	    for (int i = 0; choice[i]; i++) {
	      choice[i] = (char)tolower((unsigned char)choice[i]);
	    }
	    
	    if (strcmp(choice, "yes") == 0) {
	      // Ask for new sheduling policy
	      printf("Choose a new scheduling policy (0 - FCFS, 1 - SJF, 2 - Priority)\n> ");
	      int new_policy;
	      if(scanf("%d", &new_policy) != 1) {
		error_message("Invalid input.");
		while(getchar() != '\n');
		continue;
	      }
	      // Flush leftover input
	      while (getchar() != '\n');

	      apply_scheduling_policy(new_policy);
              printf("New scheduling policy applied. Type 'start' to execute jobs again.\n");
	    }
	    else if (strcmp(choice, "no") == 0) {
	      
	      // Clear all jobs before returning to the menu
	      clear_job_queue();

	      // Submit New Jobs
	      initialize_job_submission();
	      
	      // Re-display main menu
	      display_menu();
	      continue;
	    }
	    else {
	      // If user typed something else
	      error_message("Please type 'yes' or 'no'.");
	    }
	}

	//----------------
	// List-SUBMITTED
	//----------------
        else if (strcmp(command, "list-submitted") == 0) {
            list_jobs(0);
        }

	//---------------
	// List-SCHEDULED
	//---------------
        else if (strcmp(command, "list-scheduled") == 0) {
            list_jobs(1);
        }

	//--------------
	// QUIT
	//--------------
        else if (strcmp(command, "quit") == 0) {
            printf("Exiting Aubatch.\n");
            return 0;
        }

	//------------------
	// Unknow Commands
	//-----------------
        else {
            error_message("Unknown command. Try again.");
        }
    }

    return 0;
}

