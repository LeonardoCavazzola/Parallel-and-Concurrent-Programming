#include<stdio.h>
#include<unistd.h>
#include<sys/shm.h>

int main() {
    int key = 7;
    int shmid = shmget(key, 27 * sizeof(char), IPC_CREAT | 0600);
    char *s = shmat(shmid, 0, 0);

    int id = fork();
    if(id > 0){
        wait(NULL);
        printf("%s\n", s);
        shmdt(s); //atribui null ao ponteiro
        shmctl(shmid, IPC_RMID, NULL);
    } else {
        int i;
        for(i=0;i<26;i++){
            s[i] = 'A' + i;
        }
        shmdt(s);
    }

	return 0;
}
