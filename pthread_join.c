#include <stdio.h>
#include <pthread.h>

void *do_sum(void *); //스레드가 수행할 함수
//인자로 받은 수까지의 합을 출력

int main() {
	pthread_t thread_id[3];
	int status[3];
	int a = 4; //첫번째 스레드의 인자
	int b = 5; //두번째 스레드의 인자
	int c = 6; //세번째 스레드의 인자

	status[0] = pthread_create(&thread_id[0], NULL, do_sum, (void *)&a);
	status[1] = pthread_create(&thread_id[1], NULL, do_sum, (void *)&b);
	status[2] = pthread_create(&thread_id[2], NULL, do_sum, (void *)&c); //인자값을 포인터로 넘겨야 함
	pthread_join(thread_id[0], (void **)&status[0]);
	pthread_join(thread_id[1], (void **)&status[1]);
	pthread_join(thread_id[2], (void **)&status[2]);
	printf("Main thread END\n");
}

void *do_sum(void *arg) {
	int max = *((int *)arg);
	int i, sum = 0;
	for(i=0; i<=max; i++) {
		sum += i;
		printf("[%d] - ADD %d\n", max, i);	
	}
	printf("1부터 %d까지  합은 %d입니다.\n",max ,sum);
}
