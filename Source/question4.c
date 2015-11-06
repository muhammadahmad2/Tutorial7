
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>

int main (void) {
	pid_t pid;
	
	pid = fork();
	if (pid == 0) {
		execl("./process", 0);
		return 0;
	}
	sleep(5);
	kill(pid, SIGTSTP);
	sleep(10);
	kill(pid, SIGCONT);
	waitpid(pid, NULL, 0);
}