#include "AUbatch.h"

void display_menu() {
  printf("\n========== AUbatch Scheduler Menu ==========\n");
  printf("\nAvailable Commands:\n");
  printf(" run - Submit a job or multiple jobs to submission queue.\n");
  printf("start <policy> - Apply scheduling policy and move jobs to scheduled queue.\n");
  printf("list-submitted - List jobs in submission queue.\n");
  printf("list-scheduled - List jobs in scheduled queue.\n");
  printf(" fcfs | sjf | priority - Change scheduling policy.\n");
  printf(" quit  - Exit the program.\n\n");
  printf("==============================================\n");
}


int main() {
  char command[256];
  
  initialize_scheduler(); // Initialize the scheduler and create threads
  display_menu();
  
  while (1) {
    printf("\n> ");
    fgets(command, sizeof(command), stdin); // Read user input
    command[strcspn(command, "\n")] = 0; // Remove newline

	// Check the command
    if (strncmp(command, "run", 3) == 0){
        char name[256];
	int exec_time, priority;
	if (sscanf(command + 4, "%s %d %d", name, &exec_time, &priority) ==3) {
	  submit_job(name, exec_time, priority);
	} else {
	  error_message("Invalid format! Use run <job_name> <exec_time> <priority>");
	}
    }
    else if (strncmp(command, "start", 5) == 0) {
      int policy;
      if(sscanf(command + 6, "%d", &policy) == 1) {
	apply_scheduling_policy(policy);
      } else {
	error_message("Invalid policy! Use: start <0 (FCFS | 1 (SJF) | 2 (Priority)>");
      }
    } else if (strcmp(command, "list-submitted") == 0) {
      list_jobs(0);
    } else if (strcmp(command, "list-scheduled") == 0) {
      list_jobs(1);
    } else if (strcmp(command, "quit") == 0) {
      print_performance_metrics();
      printf("Exiting Aubatch.\n");
      break;
    } else {
      error_message("Unknown command. Try again.");
    }
  }

  return 0;
}
