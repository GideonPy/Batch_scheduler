#include "AUbatch.h"

void execute_job(job_t *job) {
  printf("Executing job: %s (Time: %d sec, Priority: %d)\n", job->name, job->execution_time, job->priority);

  // Log that job is starting (for performance metrics):
  record_job_start(job);

  // Simulate execution time
  sleep(job->execution_time);

  // Log that job is finished (for performance metrics):
  record_job_finish(job);
}

void *dispatcher_thread(void *arg) {
  while (1) {
    pthread_mutex_lock(&job_queue_mutex);

    //wait until there are jobs in the scheduled queue
    while (!scheduled_queue_head) {
      pthread_cond_wait(&job_available, &job_queue_mutex);
    }

    //Execute jobs currently in the scheduled queue
    while (scheduled_queue_head) {
      job_t *job = scheduled_queue_head;
      scheduled_queue_head = scheduled_queue_head->next;
      
      // Unlock while executing to avoid blocking scheduler
      pthread_mutex_unlock(&job_queue_mutex);

       // Simulate job execution
       execute_job(job);

       // Re-lock to modify shared data.
       pthread_mutex_lock(&job_queue_mutex);

       // Move jobs to Submission Queue
       job->next = submission_queue_head;
       submission_queue_head = job;
  }

    pthread_mutex_unlock(&job_queue_mutex);
  }
  return NULL;
}


