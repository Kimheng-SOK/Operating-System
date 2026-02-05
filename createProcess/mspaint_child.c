#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdio.h>

int main(void) {
    STARTUPINFOA si = {0};
    PROCESS_INFORMATION pi = {0};
    si.cb = sizeof(si);

    // Create a Job Object so child dies when parent exits
    HANDLE job = CreateJobObjectA(NULL, NULL);
    if (!job) {
        fprintf(stderr, "CreateJobObject failed: %lu\n", GetLastError());
        return 1;
    }

    JOBOBJECT_EXTENDED_LIMIT_INFORMATION jeli = {0};
    jeli.BasicLimitInformation.LimitFlags = JOB_OBJECT_LIMIT_KILL_ON_JOB_CLOSE;
    if (!SetInformationJobObject(job, JobObjectExtendedLimitInformation,
                                 &jeli, sizeof(jeli))) {
        fprintf(stderr, "SetInformationJobObject failed: %lu\n", GetLastError());
        CloseHandle(job);
        return 1;
    }

    char cmdLine[] = "mspaint.exe";
    if (!CreateProcessA(
            NULL, cmdLine, NULL, NULL, FALSE,
            CREATE_SUSPENDED, NULL, NULL, &si, &pi)) {
        fprintf(stderr, "CreateProcess failed: %lu\n", GetLastError());
        CloseHandle(job);
        return 1;
    }

    if (!AssignProcessToJobObject(job, pi.hProcess)) {
        fprintf(stderr, "AssignProcessToJobObject failed: %lu\n", GetLastError());
        TerminateProcess(pi.hProcess, 1);
        CloseHandle(pi.hThread);
        CloseHandle(pi.hProcess);
        CloseHandle(job);
        return 1;
    }

    ResumeThread(pi.hThread);

    CloseHandle(pi.hThread);
    CloseHandle(pi.hProcess);

    // Keep parent alive so you can see the tree; close parent to kill mspaint
    Sleep(INFINITE);

    CloseHandle(job);
    return 0;
}