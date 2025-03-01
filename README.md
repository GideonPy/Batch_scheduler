# AUbatch - A Pthread-Based Batch Scheduling System

## 📌 Project Overview
AUbatch is a **multi-threaded batch scheduling system** written in **C** using **POSIX threads (`pthread`)**.  
It implements:
- A **two-queue system** for **job submission** and **scheduled execution**.
- Three **scheduling algorithms**: **FCFS, SJF, and Priority Scheduling**.
- **Thread synchronization** using `pthread_mutex_t` and `pthread_cond_t`.
- **Real job execution** via `execv()`.
- **Performance metrics tracking** for evaluation.

---

## 🛠️ Features
✅ **Two-Queue System**  
- **Submission Queue**: Stores jobs in FIFO order.  
- **Scheduled Queue**: Jobs move here when scheduling starts, sorted by the selected policy.

✅ **Multi-Threaded Execution**  
- **Scheduler Thread**: Organizes jobs based on policy.  
- **Dispatcher Thread**: Executes jobs sequentially.

✅ **Three Scheduling Policies**  
1️⃣ **FCFS** (First Come First Serve)  
2️⃣ **SJF** (Shortest Job First)  
3️⃣ **Priority Scheduling**  

✅ **Performance Tracking**  
- **Turnaround Time**  
- **CPU Time**  
- **Waiting Time**  
- **Throughput (jobs/sec)**  

✅ **Thread Synchronization**  
- **Mutex Locks** prevent race conditions in the job queue.  
- **Condition Variables** manage job execution.

✅ **Command-Line Interface (CLI)**  
- Submit, list, and execute jobs via terminal.

---

## 📂 Project Structure
| File | Purpose |
|------|---------|
| `main.c` | Handles user commands, initializes scheduler. |
| `scheduler.c` | Manages job queue, sorts jobs, applies scheduling policy. |
| `dispatcher.c` | Executes jobs using `execv()`. |
| `job_queue.c` | Handles job queue operations (insert, delete, list). |
| `performance.c` | Tracks and reports performance metrics. |
| `utils.c` | Helper functions for logging, error handling, validation. |
| `AUbatch.h` | Header file defining structures and function prototypes. |

---

## 💻 Installation & Compilation
1. **Clone the repository**:
   ```sh
   git clone https://github.com/GideonPy/aubatch_scheduler_v2.git
   cd <new-repo-name>

## License
This project is licensed under the no License. 

##Author
Henry Porter - GideonPy
