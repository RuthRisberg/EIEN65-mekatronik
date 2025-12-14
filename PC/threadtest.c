#include <stdio.h>
#include <threads.h>

int read_thread_func(void* ignored)
{
    // int i;
    // scanf("%d\n", &i);

    int ch = getchar();
    int ch2 = getchar();

    printf("read:-%c-%c-\n", ch, ch2);
}

int main()
{
    thrd_t read_thread;
    int temp;

    temp = thrd_create(&read_thread, read_thread_func, NULL);

    thrd_sleep(&(struct timespec){.tv_sec=10}, NULL);

    printf("\roriginal thread\n");
}
