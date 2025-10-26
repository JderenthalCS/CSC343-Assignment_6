# Assignment 6 — CSC343 Operating Systems

This assignment demonstrates basic thread synchronization using **mutex locks** and **condition variables** with **Pthreads** in C.  
The program creates two threads — a **writer** that updates shared data and a **reader** that waits and reads it safely without conflicts.

## How to Compile and Run
```bash
gcc -pthread Q4_mutex_pthread.c -o Q4_mutex_pthread
./Q4_mutex_pthread
