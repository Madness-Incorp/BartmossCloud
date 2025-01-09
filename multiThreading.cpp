#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include "setuppage.h"
extern "C" {
    int checkIfLocationsSet();
}

void launchClient() {
    std::cout << "Starting Client process..." << std::endl;
    execl("./Client", "Client", NULL);
    perror("Error with execl for Client");
    exit(EXIT_FAILURE);
}

void launchBartmossCloud() {
    std::cout << "Starting BartmossCloud process..." << std::endl;
    execl("./BartmossCloud", "BartmossCloud", NULL);
    perror("Error with execl for BartmossCloud");
    exit(EXIT_FAILURE);
}

void runSetUpPage() {
    std::cout << "Starting Setup process..." << std::endl;
    execl("./SetUpPage", "SetUpPage", NULL);
    perror("Error with execl for SetUpPage");
    exit(EXIT_FAILURE);
}

int main() {
    if(checkIfLocationsSet() == 0) {
        runSetUpPage();
    }else {
        std::cout << "Setup already completed, proceeding to multithreading..." << std::endl;
    }

    const pid_t pid1 = fork();
    if(pid1 < 0) {
        perror("Error with fork() for Client");
        exit(EXIT_FAILURE);
    }

    if(pid1 == 0) {
        launchClient();
    }

    const pid_t pid2 = fork();
    if(pid2 < 0) {
        perror("Error with fork() for Client GUI");
        exit(EXIT_FAILURE);
    }

    if(pid2 == 0) {
        launchBartmossCloud();
    }

    int status1, status2;
    waitpid(pid1, &status1, 0);
    waitpid(pid2, &status2, 0);

    if(WIFEXITED(status1) && WIFEXITED(status2)){
        std::cout << "Both Client and BartmossCloud processes completed successfully." << std::endl;
    } else {
        std::cout << "One or both processes did not complete successfully." << std::endl;
    }

    return 0;
}