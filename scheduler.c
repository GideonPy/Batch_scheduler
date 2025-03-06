#include "AUbatch.h"

// Mutex & Condition Variables (global)
pthread_mutex_t job_queue_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t job_available = PTHREAD_COND_INITIALIZER;

// Global Queue Heads
job_t *submission_queue_head = NULL;
job_t *scheduled_queue_head = NULL;
int scheduling_policy = 0;

// Creates and detaches the scheduler and dispatcher threads.
void initialize_scheduler() {
  pthread_t scheduler_thread_id, dispatcher_thread_id;

  // Create scheduler
  if (pthread_create(&scheduler_thread_id, NULL, scheduler_thread, NULL) != 0) {
    error_message("Failed to create scheduler thread.");
    return;
  }

  // Create dispather thread
  if (pthread_create(&dispatcher_thread_id, NULL, dispatcher_thread, NULL) != 0) {
    error_message("Failed to create dispatcher thread.");
    return;
  }

  // Detach threads so they run independently
  pthread_detach(scheduler_thread_id);
  pthread_detach(dispatcher_thread_id);

  printf("Scheduler and dispatcher threads initialized successfully.\n");
}

// Waits for jobs to become available in the Scheduled Queue.
void *scheduler_thread(void *arg) {
  while(1) {
    pthread_mutex_lock(&job_queue_mutex);

    //Wait if no jobs are available
    while(!scheduled_queue_head) {
      pthread_cond_wait(&job_available, &job_queue_mutex);
    }
    // Unlock before doing "work"
    pthread_mutex_unlock(&job_queue_mutex);

    //Simulate some periocic scheduling action
    sleep(1);
  }
  return NULL;
}

// Sets global scheduling policy and reorders jobs accordingly. 
void apply_scheduling_policy(int policy) {
  pthread_mutex_lock(&job_queue_mutex);

  // Ensure a valid policy
  if (policy < 0 || policy > 2) {
    error_message("Invalid policy! Use: start < 0 (FCFS | 1 (SJF) | 2 (Priority)>");
    pthread_mutex_unlock(&job_queue_mutex);
    return;
  }

  scheduling_policy = policy;
  printf("Scheduling policy set to: %d\n", scheduling_policy);

  //If jobs are already in Scheduling Queue, just re-order them
  if (scheduled_queue_head) {
    printf("Reodering jobs in Scheduled Queue based on new policy.\n");
    job_t *sorted_queue = NULL;
    
    while (scheduled_queue_head) {
      job_t *job = scheduled_queue_head;
      scheduled_queue_head = scheduled_queue_head->next;
      job->next = NULL;
      insert_sorted(&sorted_queue, job, scheduling_policy);
    }
    scheduled_queue_head = sorted_queue;
  }
  // If no jobs in Scheduling Queue, move from Submission Queue
  else if (submission_queue_head) {
    printf("Moving jobs from submission Queue to scheduled Queue.\n");
  while (submission_queue_head) {
    job_t *job = submission_queue_head;
    submission_queue_head = submission_queue_head->next;
    job->next = NULL; // Reset next pointer before inserting
    insert_sorted(&scheduled_queue_head, job, scheduling_policy);
  }
  }
  else {
    error_message("No jobs available in either Submission or Scheduled Queue.");
  }

  pthread_cond_signal(&job_available);
  pthread_mutex_unlock(&job_queue_mutex);
}
// Insert jos into queue according to current schedule policy.
void insert_sorted(job_t **queue, job_t *job, int policy) {
  // If the queue is empty or if job should come before the first element.
  if (!*queue || compare_jobs(job, *queue, policy) < 0) {
    job->next = *queue;
    *queue = job;
  } else {
    // Traverse until we find the correct insertion point.
    job_t *temp = *queue;
    while (temp->next && compare_jobs(job, temp->next, policy) >=0) {
      temp = temp->next;
    }
    job->next = temp->next;
    temp->next = job;
  }
}

int compare_jobs(job_t *a, job_t *b, int policy) {
  if (policy == 0) return a->arrival_time.tv_sec - b->arrival_time.tv_sec; // FCFS
  if (policy == 1) return a->execution_time - b->execution_time; // SJF
  return b->priority - a->priority; // Priority
}
