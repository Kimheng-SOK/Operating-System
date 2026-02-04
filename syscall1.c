#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>

#define BUFFER_SIZE 4096

int main() {
    int source_fd, dest_fd;
    ssize_t bytes_read, bytes_written;
    char buffer[BUFFER_SIZE];

    // Open source file for reading
    source_fd = open("result.txt", O_RDONLY);
    if (source_fd == -1) {
        perror("Error opening source file");
        exit(1);
    }

    // Open destination file for writing (create if doesn't exist, truncate if exists)
    dest_fd = open("copyresult.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (dest_fd == -1) {
        perror("Error opening destination file");
        close(source_fd);
        exit(1);
    }

    // Copy file content
    while ((bytes_read = read(source_fd, buffer, BUFFER_SIZE)) > 0) {
        bytes_written = write(dest_fd, buffer, bytes_read);
        if (bytes_written != bytes_read) {
            perror("Error writing to destination file");
            close(source_fd);
            close(dest_fd);
            exit(1);
        }
    }

    if (bytes_read == -1) {
        perror("Error reading from source file");
        close(source_fd);
        close(dest_fd);
        exit(1);
    }

    // Close files
    close(source_fd);
    close(dest_fd);

    printf("File copied successfully from result.txt to copyresult.txt\n");
    return 0;
}
