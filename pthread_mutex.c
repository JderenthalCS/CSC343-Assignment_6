#include <pthread.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>

// Shared data and synchronization tools
pthread_mutex_t dataLock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t dataReadySignal = PTHREAD_COND_INITIALIZER;

int sharedNumber = 0;
bool hasNewData = false;
bool writerFinished = false;

// Writer thread: creates data and notifies reader
void* writerThread(void* arg) {
    for (int i = 1; i <= 5; i++) {
        pthread_mutex_lock(&dataLock);

        sharedNumber = i * 10;
        hasNewData = true;
        printf("[Writer] Wrote number: %d\n", sharedNumber);

        pthread_cond_signal(&dataReadySignal);
        pthread_mutex_unlock(&dataLock);

        sleep(1); // Simulate work
    }

    pthread_mutex_lock(&dataLock);
    writerFinished = true;
    pthread_cond_signal(&dataReadySignal);
    pthread_mutex_unlock(&dataLock);

    return NULL;
}

// Reader thread: waits for new data and reads it
void* readerThread(void* arg) {
    while (true) {
        pthread_mutex_lock(&dataLock);

        while (!hasNewData && !writerFinished) {
            pthread_cond_wait(&dataReadySignal, &dataLock);
        }

        if (writerFinished && !hasNewData) {
            pthread_mutex_unlock(&dataLock);
            break;
        }

        printf("[Reader] Read number: %d\n", sharedNumber);
        hasNewData = false;

        pthread_mutex_unlock(&dataLock);
    }

    return NULL;
}

// Main function: starts threads and waits for them to finish
int main() {
    pthread_t writer, reader;

    pthread_create(&writer, NULL, writerThread, NULL);
    pthread_create(&reader, NULL, readerThread, NULL);

    pthread_join(writer, NULL);
    pthread_join(reader, NULL);

    printf("All done!\n");
    return 0;
}
