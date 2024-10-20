#include <stdio.h>
#include <stdlib.h>
#include <sys/_types/_pid_t.h>
#include <sys/wait.h>
#include <unistd.h>

int main(void) {

  pid_t pid1, pid2;

  pid1 = fork();

  if (pid1 < 0) {
    perror("Error with fork()");
    exit(EXIT_FAILURE);
  }

  if (pid1 == 0) {

    execl("./client", "./client", NULL);
    perror("Error with execl");
    exit(EXIT_FAILURE);
  } else {
    pid2 = fork();
    if (pid2 < 0) {
      perror("Fork failed!!");
      exit(EXIT_FAILURE);
    }

    if (pid2 == 0) {
      execl("./BartmossCloud", "./BartmossCloud", NULL);

      perror("Excl failed");
      exit(EXIT_FAILURE);
    }
  }

  wait(&pid1);
  wait(&pid2);
  

  return 0;
}
