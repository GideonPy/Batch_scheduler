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

// A link list node for storing "Job X completed." messages.
typedef struct completion_node {
  char msg[256];
  struct completion_node *next;
} completion_node_t;

// Head pointer for the linked list of completion messages
static completion_node_t *completion_head = NULL;

// Mutex to protect insertion & removal of completion messages
static pthread_mutex_t completion_mutex = PTHREAD_MUTEX_INITIALIZER;


// Called by the dispatcher to store

void record_job_completion(const char *jobName)
{
  // Build the message
  char buffer[256];
  snprintf(buffer, sizeof(buffer), "Job %s completed.", jobName);

  //Create a new node
  completion_node_t *node = (completion_node_t *)malloc(sizeof(*node));
  if (!node) {
    // If out of memory, optionally log or handle error
    return;
  }
  strncpy(node->msg, buffer, sizeof(node->msg));
  node->msg[sizeof(node->msg) - 1] = '\0'; // ensure null termination
  node->next = NULL;

  // Insert into single-linked list
    pthread_mutex_lock(&completion_mutex);

    if (!completion_head) {
      completion_head = node;
    } else {
      completion_node_t *cur = completion_head;
      while (cur->next) {
	cur = cur->next;
      }
      cur->next = node;
    }

    pthread_mutex_unlock(&completion_mutex);
}

// Called by main thread (after all jobs in queue completed)
// Avoid concurrency issues with main thread

void flush_completion_messages(void)
{
  pthread_mutex_lock(&completion_mutex);
  completion_node_t *cur = completion_head;
  completion_head = NULL; // detach the entire list
  pthread_mutex_unlock(&completion_mutex);

  // Print and free the messages
  while (cur) {
    printf("%s\n", cur->msg);
    completion_node_t *tmp = cur;
    free(tmp);
  }
}
