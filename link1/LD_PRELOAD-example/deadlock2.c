#include <pthread.h>
#include <stdio.h>

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
int global_shared;

static void *
thread1(void * _tn)
{
    pthread_mutex_lock(&lock);
    global_shared++;
    pthread_mutex_lock(&lock);
    return NULL;
}

static void *
thread2(void * _tn)
{
    pthread_mutex_lock(&lock);
    global_shared++;
    pthread_mutex_unlock(&lock);
    return NULL;
}

int
main()
{
    int i, N = 2;
    pthread_t t[N];
    pthread_create(t + 0, NULL, thread1, NULL);
    pthread_create(t + 1, NULL, thread2, NULL);

    for (i = 0; i < N; i++)
        pthread_join(t[i], NULL);

    printf("global_shared = %d\n", global_shared);
    return 0;
}
