#include "AUbatch.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void initialize_job_submission() {
  char job_input[1024];

  printf("\nEnter jobs in format: <job_name> <exec_time> <priority>, seperated by commas\n");
  printf("Example: job1 10 3, job2 5 2, job3 7 1\n> ");

  if(!fgets(job_input, sizeof(job_input), stdin)) {
    error_message("Failed to read jobs. Try again.");
    return;
  }

  if (strlen(job_input) <= 1) {
    error_message("No jobs entered. Please try again.");
  return;
}

int job_count = 0;
char *token = strtok(job_input, ",");
while (token != NULL) {
  char name[256];
  int exec_time, priority;
  if (sscanf(token, "%255s %d %d", name, &exec_time, &priority) == 3) {
    submit_job(name, exec_time, priority);
    job_count++;
  }else {
    printf("[ERROR]: Invalid job format: %s\n", token);
  }
  token = strtok(NULL, ",");
  
 }

if (job_count == 0) {
  error_message("No vaild jobs entered.");
 } else {
  printf("Successfully added %d jobs to the Submission Queue.\n", job_count);
 }
}
  

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

void clear_job_queue() {
  pthread_mutex_lock(&job_queue_mutex);

  // Clear Scheduled Queue() {
  while (scheduled_queue_head) {
    job_t *temp = scheduled_queue_head;
    scheduled_queue_head = scheduled_queue_head->next;
    free(temp);
  }

  // Clear Submitted Queue
  while (submission_queue_head) {
    job_t *temp = submission_queue_head;
    submission_queue_head = submission_queue_head->next;
    free(temp);
  }

  printf("All jobs cleard from both queues.\n");
  pthread_mutex_unlock(&job_queue_mutex);
}
