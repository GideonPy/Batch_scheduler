#ifndef AUBATCH_H
#define AUBATCH_H

#include <stdio.h>    // Standard I/O functions
#include <stdlib.h>   // Memory management functions
#include <string.h>
#include <pthread.h>  // Provides threading support
#include <unistd.h>   // UNIX standard symbolic constants and types
#include <sys/types.h> // Defines data types used in system calls
#include <sys/wait.h> // Provides macros related to process control
#include <sys/time.h> // Provides time functions
 

// Job Structure
typedef struct job {
    char name[256];  // Name of the job
    int execution_time; // Expected execution time in seconds
    int priority; // Job priority level
    struct timeval arrival_time; // Timestamp when the job was submitted
    struct timeval start_time;   // Timestamp when job starts execution
    struct timeval finish_time;  // Timestamp when job completes execution
    struct job *next; // Pointer to the next job in the queue
} job_t;

// Global Variables (Declared as extern)
extern job_t *submission_queue_head; // Stores submitted jobs
extern job_t *scheduled_queue_head;  // Stores sorted jobs after scheduling
extern pthread_mutex_t job_queue_mutex;
extern pthread_cond_t job_available;
extern int scheduling_policy;
extern void flush_completion_messages(void); // Print saved completion messages after main thread

// Function Prototypes
void initialize_job_submission();
void submit_job(const char *name, int execution_time, int priority);
void list_jobs(int queue_type);
void apply_scheduling_policy(int policy);
void execute_job(job_t *job);
void clear_job_queue();
void initialize_scheduler();
void *scheduler_thread(void *arg);
void *dispatcher_thread(void *arg);

// Insertion/Comparision for scheduling
void insert_sorted(job_t **queue, job_t *job, int policy);
int compare_jobs(job_t *a, job_t *b, int policy);

// Logging Helpers
void log_message(const char *message);
void error_message(const char *message);

// Queue Management
void enqueue_submission_queue(job_t *job);

// Performance Manager
void record_job_start(job_t *job);
void record_job_finish(job_t *job);
void print_performance_metrics();

#endif

