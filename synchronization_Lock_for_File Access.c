#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <errno.h>

void write_message(const char *message, int count) {
    for (int i = 0; i < count; i++) {
        printf("%s\n", message);
        usleep((rand() % 100) * 1000); // Random delay between 0 and 99 milliseconds
    }
}

// function to ensure that only one process can write to the file at a time.
void acquiring_lock() {
    int fd;
    // file named lockfile.lock if it is not exists (O_CREAT).
    // ensire open will fail if the file already exists - other process created the file (O_EXCL).
    // loop tries to create the lock file until the file is successfully created.
    while ((fd = open("lockfile.lock", O_CREAT | O_EXCL)) == -1) {
        // if the file exists, another process is currently writing, so wait to try again.
        if (errno == EEXIST) {
            usleep(100000);
        } else {
            // if other error, print error and exit.
            perror("Failed to create lockfile");
            exit(EXIT_FAILURE);
        }
    }
    close(fd);
}

// function to remove the lock file.
void release_lock() {
    // after the process completes its writing, it deletes the lock file.
    if (unlink("lockfile.lock") == -1) {
        // if fail to remove lockfile, print error and exit.
        perror("Failed to remove lockfile");
        exit(EXIT_FAILURE);
    }
}

int main(int argc, char *argv[]) {
    // ensure the program accepts at least 3 messages and the count of writes.
    if (argc <= 4) {
        fprintf(stderr, "Usage: %s <message1> <message2> ... <count>", argv[0]);
        return 1;
    }

    // last command-line argument represents the number of times each message should be printed.
    int count = atoi(argv[argc - 1]);
    // calculate the number of messages (not including the program name and the count of writes).
    int num_messages = argc - 2;
    // create an array of pid_t to store the IDs of the child processes.
    pid_t pids[num_messages];

    // create a child process for each message using fork.
    for (int i = 0; i < num_messages; i++) {
        // if fork failed, print error and exit.
        if ((pids[i] = fork()) == -1) {
            perror("Fork failed");
            exit(EXIT_FAILURE);
        } else if (pids[i] == 0) { 
            acquiring_lock();
            write_message(argv[i + 1], count);
            release_lock(); 
            exit(0); // exit child process.
        }
    }

    // parent process waits for all child processes to complete their writing.
    for (int i = 0; i < num_messages; i++) {
        waitpid(pids[i], NULL, 0);
    }
    
    return 0;
}
