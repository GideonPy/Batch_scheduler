#include "AUbatch.h"

// Performance Metrics
int total_jobs = 0;
int completed_jobs = 0;
double total_turnaround_time = 0;
double total_cpu_time = 0;
double total_waiting_time = 0;

// Records job start time
void record_job_start(job_t *job) {
  gettimeofday(&job->start_time, NULL);
}

// Records job completion time and updates perfomance metrics
void record_job_finish(job_t *job) {
  gettimeofday(&job->finish_time, NULL);

  double turnaround_time = (job->finish_time.tv_sec - job->arrival_time.tv_sec) + (job->finish_time.tv_usec - job->arrival_time.tv_usec) / 1e6;

  double waiting_time = (job->start_time.tv_sec - job->arrival_time.tv_sec) + (job->start_time.tv_usec - job->arrival_time.tv_usec) /1e6j;

  total_turnaround_time += turnaround_time;
  total_waiting_time += waiting_time;
  total_cpu_time += job->execution_time;
  completed_jobs++;
  
}

//Prints performance metrics at the end of execution
void print_performance_metrics() {
  if (completed_jobs == 0) {
    printf("\nNo jobs completed yet.\n");
    return;
  }

  printf("\n--- Performance Metrics ---\n");
  printf("Total Jobs Completed: %d\n", completed_jobs);
  printf("Average Turnaround Time: %.2f seconds\n", total_turnaround_time / completed_jobs);
  printf("Average CPU Time: %.2f seconds\n", total_cpu_time / completed_jobs);
  printf("Average Waiting Time: %.2f seconds\n", total_waiting_time /completed_jobs);
  printf("Throughput: %.2f jobs/seconds\n", (double)completed_jobs / total_turnaround_time);
  printf("-----------------------------\n");

}
