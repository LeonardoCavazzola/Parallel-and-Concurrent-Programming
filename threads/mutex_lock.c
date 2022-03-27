#include<stdio.h>
#include<pthread.h>

#define NUMBER_OF_THREADS 10

int cont = 0;
pthread_mutex_t m;

void * execution(void * args) {
	int i, c=0;
	for (i=0; i<10000; i++){	
		c++;
	}
	pthread_mutex_lock(&m);
	cont += c;
	pthread_mutex_unlock(&m);
}

int main() {
	int i,id[T];
	pthread_t tid[T];
	pthread_mutex_init(&m, NULL);
	
	for (i=0; i<T; i++) {
		id[i]=i;
		pthread_create(&tid[i], NULL, execution, (void*) &id[i]);
	}
	
	for (i=0; i<T; i++) {
		pthread_join(tid[i], NULL);
	}
	
	pthread_mutex_destroy(&m);
	printf("%d", cont);
	return 0;
}
