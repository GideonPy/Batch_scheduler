#include "AUbatch.h"

void *dispatcher_thread(void *arg) {
  while (1) {
    pthread_mutex_lock(&job_queue_mutex);
    while (!scheduled_queue_head) {
      pthread_cond_wait(&job_available, &job_queue_mutex);
    }

    job_t *job = scheduled_queue_head;
    scheduled_queue_head = scheduled_queue_head->next;

    pthread_mutex_unlock(&job_queue_mutex);

    record_job_start(job);
    execute_job(job);
    record_job_finish(job);
  }
  return NULL;
}

void execute_job(job_t *job) {
  printf("Executing job: %s (Time: %d sec, Priority: %d)\n", job->name, job->execution_time, job->priority);
  sleep(job->execution_time);
  printf("Job %s completed.\n", job->name);
}
