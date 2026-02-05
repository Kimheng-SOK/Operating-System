#include <windows.h>
#include <stdio.h>

#define N 5

DWORD WINAPI worker(LPVOID param) {
    int id = (int)(intptr_t)param;
    printf("Thread %d running (tid=%lu)\n", id, GetCurrentThreadId());
    Sleep(100);
    return 0;
}

int main() {
    HANDLE threads[N];

    for (int i = 0; i < N; i++) {
        threads[i] = CreateThread(NULL, 0, worker, (LPVOID)(intptr_t)i, 0, NULL);
    }
    WaitForMultipleObjects(N, threads, TRUE, INFINITE);
    for (int i = 0; i < N; i++) {
        CloseHandle(threads[i]);
    }
    return 0;
}