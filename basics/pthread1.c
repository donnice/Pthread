#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <errno.h>

void *print_message_function(void *ptr);

main()
{
    pthread_t thread1, thread2;
    const char *message1 = "Thread 1";
    const char *message2 = "Thread 2";
    int iter1, iter2;

    /* Create independent threads each of which will 
     * execute function
     */

    iter1 = pthread_create(&thread1, NULL, print_message_function, (void *)message1);
    if(iter1)
    {
        fprintf(stderr, "Error - pthread_create() return code: %d\n",iter1);
        exit(EXIT_FAILURE);
    }

    iter2 = pthread_create(&thread2, NULL, print_message_function, (void *)message2);
    if(iter2)
    {
        fprintf(stderr, "Error - pthread_create() return code: %d\n",iter1);
        exit(EXIT_FAILURE);
    }

    printf("pthread_create() for thread 1 returns: %d\n", iter1);
    printf("pthread_create() for thread 2 returns: %d\n", iter2);    

    /* Wait till threads complete before main continues.
     * Unless we wait we run the risk of executing an exit
     * which will terminate the process and all threads
     * before the threads have completed.
     */
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    exit(EXIT_SUCCESS);
}

void *print_message_function(void *ptr)
{
    char *message;
    message = (char *)ptr;
    printf("%s \n", message);
}
    
