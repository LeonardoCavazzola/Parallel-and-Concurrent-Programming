#include<stdio.h>
#include<signal.h>
#include<unistd.h>

int using_fork() {
	int id = fork();
	if (id > 0){
		printf("PAI: pid %d\n", getpid());
		sleep(5);
		kill(id, SIGSTOP);
		sleep(5);
		kill(id, SIGCONT);
		sleep(5);
		kill(id, SIGKILL);
	} else {
		printf("FILHO: pid %d\n", getpid());
	}
	return 0;
}

int main() {
    using_fork();
	return 0;
}
