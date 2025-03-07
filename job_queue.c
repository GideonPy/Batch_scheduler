#include "AUbatch.h"

void submit_job(const char *name, int execution_time, int priority) {
  pthread_mutex_lock(&job_queue_mutex);

  // Allocate memory for the new job
  job_t *new_job = (job_t *)malloc(sizeof(job_t));
  if (!new_job) {
    error_message("Memory allocation failed.");
    pthread_mutex_unlock(&job_queue_mutex);
    return;
  }

  strcpy(new_job->name, name);
  new_job->execution_time = execution_time;
  new_job->priority = priority;
  gettimeofday(&new_job->arrival_time, NULL);
  new_job->next = NULL;

  // Insert job into Submission Queue (FIFO Order)
  if (!submission_queue_head) {
    submission_queue_head = new_job;
  } else {
    job_t *temp = submission_queue_head;
    while (temp->next) {
      temp = temp->next;
    }
    temp->next = new_job;
  }

  printf("Job %s added to submission queue.\n", name);
  pthread_mutex_unlock(&job_queue_mutex);
}

void list_jobs(int queue_type){
  job_t *queue = queue_type ? scheduled_queue_head : submission_queue_head;
printf("Listing jobs in %s queue:\n", queue_type ? "Scheduled" : "Submission");
while (queue) {
  printf("Job: %s, Exec Time: %d, Priority: %d\n",
	 queue->name,
	 queue->execution_time,
	 queue->priority);
  queue = queue->next;
 }
}
