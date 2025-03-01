#include "AUbatch.h"

pthread_mutex_t job_queue_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t job_available = PTHREAD_COND_INITIALIZER;

job_t *submission_queue_head = NULL;
job_t *scheduled_queue_head = NULL;
int scheduling_policy = 0;

void initialize_scheduler() {
  pthread_t scheduler_thread_id, dispatcher_thread_id;

  // Create scheduler and dispatcher threads
  if (pthread_create(&scheduler_thread_id, NULL, scheduler_thread, NULL) != 0) {
    error_message("Failed to create scheduler thread.");
    return;
  }

  if (pthread_create(&dispatcher_thread_id, NULL, dispatcher_thread, NULL) != 0) {
    error_message("Failed to create dispatcher thread.");
    return;
  }

  // Detach threads so they run independently
  pthread_detach(scheduler_thread_id);
  pthread_detach(dispatcher_thread_id);

  printf("Scheduler and dispatcher threads initialized successfully.\n");
}

void *scheduler_thread(void *arg) {
  while(1) {
    pthread_mutex_lock(&job_queue_mutex);

    //Wait if no jobs are available
    while(!scheduled_queue_head) {
      pthread_cond_wait(&job_available, &job_queue_mutex);
    }

    pthread_mutex_unlock(&job_queue_mutex);
    sleep(1);
  }
  return NULL;
}


void apply_scheduling_policy(int policy) {
  pthread_mutex_lock(&job_queue_mutex);
  scheduling_policy = policy;

  job_t *temp = submission_queue_head;
  scheduled_queue_head = NULL;

  while (temp) {
    job_t *next = temp->next;
    insert_sorted(&scheduled_queue_head, temp, policy);
    temp = next;

  }

  submission_queue_head = NULL;
  pthread_cond_signal(&job_available);
  pthread_mutex_unlock(&job_queue_mutex);

  printf("Scheduling policy applied. Jobs moved to scheduled queue.\n");
}

void insert_sorted(job_t **queue, job_t *job, int policy) {
  if (!*queue || compare_jobs(job, *queue, policy) < 0) {
    job->next = *queue;
    *queue = job;
  } else {
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
