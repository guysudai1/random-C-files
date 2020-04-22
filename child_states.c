#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

int main() {
	
	pid_t PID;
	pid_t parentPID;
	pid_t initPID;
	initPID = fork();

	if (initPID < 0) { exit(0); }
	if (initPID == 0) {
		PID = getpid();
		parentPID = getppid();
		printf("Child PID: %d, Parent PID: %d\n", PID, parentPID);
	}
	if (initPID != 0) {
		PID = getpid();
                parentPID = getppid();
                printf("Parent PID: %d, Parent of Parent PID: %d\n", PID, parentPID);
                exit(0);
	}

	PID = getpid();
	parentPID = getppid();
	printf("After death - Child PID: %d, Parent PID: %d\n", PID, parentPID);


}
