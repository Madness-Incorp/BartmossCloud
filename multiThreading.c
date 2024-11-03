#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(void) {
    pid_t pid1, pid2;

    // Fork for the Client executable
    pid1 = fork();
    if (pid1 < 0) {
        perror("Error with fork() for Client");
        exit(EXIT_FAILURE);
    }

    if (pid1 == 0) {
        // Child process: Run the Client executable
        printf("Starting Client process...\n");
        execl("./Client", "Client", NULL);
        perror("Error with execl for Client");
        exit(EXIT_FAILURE);
    }

    // Fork for the BartmossCloud executable
    pid2 = fork();
    if (pid2 < 0) {
        perror("Error with fork() for BartmossCloud");
        exit(EXIT_FAILURE);
    }

    if (pid2 == 0) {
        // Child process: Run the BartmossCloud executable
        printf("Starting BartmossCloud process...\n");
        execl("./BartmossCloud", "BartmossCloud", NULL);
        perror("Error with execl for BartmossCloud");
        exit(EXIT_FAILURE);
    }

    // Parent process: Wait for both child processes to complete
    int status1, status2;
    waitpid(pid1, &status1, 0);
    waitpid(pid2, &status2, 0);

    if (WIFEXITED(status1) && WIFEXITED(status2)) {
        printf("Both Client and BartmossCloud processes completed successfully.\n");
    } else {
        printf("One or both processes did not complete successfully.\n");
    }

    return 0;
}
