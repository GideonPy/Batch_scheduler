#include "AUbatch.h"

// Logs a general message
void log_message(const char *message) {
  printf("[LOG]:%s\n", message);
}

// Prints and error message
void error_message(const char *message) {
  fprintf(stderr, "[ERROR]: %s\n", message);
}

// Validates if a job exsist in the current directory before execution
int is_valid_executable(const char *filename) {
  if (access(filename, F_OK) == 0) {
    return 1; // File exists
  }
  return 0; // File does not exist
}

// Converts scheduling policy from inter to readable format
const char *get_policy_name(int policy) {
  switch (policy) {
  case 0:
    return "FCFS (First Come First Serve)";
  case 1:
    return "SJF (Shortest Job First)";
  case 2:
    return "Priority-Based Scheduling";
  default:
    return "Unknown";
  }
}
