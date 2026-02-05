#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

void* worker(void* arg) {
    long id = (long)arg;
    printf("Thread %ld running (tid=%lu)\n", id, (unsigned long)pthread_self());
    usleep(100000);
    return NULL;
}

int main() {
    const int N = 5;
    pthread_t threads[N];

    for (long i = 0; i < N; i++) {
        pthread_create(&threads[i], NULL, worker, (void*)i);
    }
    for (int i = 0; i < N; i++) {
        pthread_join(threads[i], NULL);
    }
    return 0;
}
