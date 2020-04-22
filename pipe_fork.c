#include<stdio.h> 
#include<stdlib.h> 
#include<unistd.h> 
#include<sys/types.h> 
#include<string.h> 
#include<sys/wait.h> 
#include <errno.h>
#include <sys/socket.h>

int main() {
	int fd[2];
	int errfd[2];
	//char* stdin_str = "hello";
	char* stdin_str = "\x00\x0a\x00\xff";
	char* stderr_str = "\x00\x0a\x02\xff";
	// char input_str[100];
	pid_t pid;

	if (pipe(fd) < 0) {
		fprintf(stderr, "Failed piping!");
		return -1;
	}
	if (pipe(errfd) < 0) {
		fprintf(stderr, "Failed piping err!");
		return -1;
	}
	pid = fork();
	if (pid < 0) {
		fprintf(stderr, "Forking failed!");
		return -1;
	} else if (pid > 0) {
		// Stage 2
		if (dup2(fd[0], 0) < 0) {
			fprintf(stderr, "Duping failed!");
			return -1;
		}	

		if (dup2(errfd[0], 2) < 0) {
			fprintf(stderr, "Duping stderr failed!");
			return -1;
		}

		close(errfd[1]);
		close(errfd[0]);
		
		close(fd[1]);
		close(fd[0]);

		// Stage 1
		char *argv[101];	
		argv[0] = "./test";
		for (int i=1; i<100; i++)
			argv[i] = "A";
		argv['A'] = "\x00";
		argv['B'] = "\x20\x0a\x0d";
		argv[100] = '\0';
		
		// Stage 3
		char *envp[2];
		envp[0] = "\xde\xad\xbe\xef=\xca\xfe\xba\xbe";
		envp[1] = '\0';

		// Stage 4
		FILE* fp;
		fp = fopen("\x0a", "wb");
		fwrite("\x00\x00\x00\x00", 4, 1, fp);
		fclose(fp);
		
		// Stage 5
		argv['C'] = "12345";
		int port_wanted = 12345;

		printf("Port connect to: %d\n", port_wanted);
		if (execve(argv[0], argv, envp) < 0) {
			fprintf(stderr, "Error execve: %s\n", strerror(errno));
			return -1;
		}
	} else {
		
		close(fd[0]);
		write(fd[1], stdin_str, 4);
		close(fd[1]);

		close(errfd[0]);
		write(errfd[1], stderr_str, 4);
		close(errfd[1]);
		exit(0);
	}
}
