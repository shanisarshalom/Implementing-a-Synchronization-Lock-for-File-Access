# Implementing a Synchronization Lock for File Access

Implementing a synchronization lock for file access, ensuring that only one process writes to a file at a time while other processes wait. The solution uses a lock file as a mutual exclusion (mutex) mechanism to control access to the shared file.

## Objective

The goal is to synchronize file access among multiple processes using a lock file. This ensures that processes write to the file sequentially, avoiding conflicts and interleaving outputs.

## Implementation Steps

1. **File Creation**:
   - Use a separate lock file named `lockfile.lock` to manage mutual exclusion.
   - Create an output file named `output2.txt` to store the redirected output.

2. **Process Writing**:
   - Fork a dynamic number of child processes based on input arguments.
   - Each child process writes a specified message to the shared output using the `write_message` function.

3. **Synchronization Lock**:
   - **Acquiring the Lock**:
     - Before writing, a process attempts to create the lock file (`lockfile.lock`).
     - If the lock file exists, the process waits (using `usleep`) and retries until the lock is available.
   - **Releasing the Lock**:
     - After writing, the process deletes the lock file, signaling that other processes can now write.

4. **Forking and Writing**:
   - Open the file needed for writing before forking.
   - Fork the child processes in a loop, passing the necessary arguments.
   - Ensure the parent process waits for all child processes to complete.

5. **Error Handling**:
   - Use `perror` to print error messages for failed system calls (e.g., `fork`, `wait`, `usleep`).

6. **Command-Line Input**:
   - Accept arguments specifying:
     - Messages each process writes.
     - The number of times each process writes to the file.

## Input Parameters

The program accepts the following command-line arguments:

1. At least three messages, one for each process.
2. The number of times each message is written to the file.

**Example Command:**

```bash
./synchronization_Lock_for_File Access "First message" "Second message" "Third message" 3 > output.txt
