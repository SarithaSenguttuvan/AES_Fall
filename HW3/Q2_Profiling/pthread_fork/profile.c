/**
 * @file 
 *		profile.c
 * @brief 
 *		This file implements the profiling for pthread create, fork and exec functionalities
 *
 * @author 
 *		Saritha Senguttuvan
 * @date 
 *		Oct,2017
 *
 */
#define _GNU_SOURCE
#include <stdio.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <pthread.h>
#include <stdint.h>
#include <stdlib.h>




/* Thread start routine */
void *dummy_fn(void *threadid)
{
    printf("Do Nothing in thread function\n");
    pthread_exit(NULL);
}

void main()
{
    uint64_t total_time = 0;
	struct timespec usage_before;
    struct timespec usage_after;
	pid_t pid;
	pthread_t tid_1;
    pthread_attr_t t1_attr;
    size_t rc;
 
 /********************** Pthread Create **************************************************/ 
  
    pthread_attr_init(&t1_attr);            /* initializes the pthread attribute values */
    if(clock_gettime(CLOCK_THREAD_CPUTIME_ID,&usage_before))           /* Get the current time */
        printf("ERROR:clock_gettime before pthread_create \n");
    rc = pthread_create(&tid_1, &t1_attr, dummy_fn, NULL);  /* Call to create a new thread */
    if(clock_gettime(CLOCK_THREAD_CPUTIME_ID,&usage_after))           /* Get the current time */
        printf("ERROR:clock_gettime before pthread_create \n");
    if (rc) 
    {
        printf("ERROR; pthread_create() rc is %ld\n", rc); 
        perror(NULL); exit(-1);
    }
    printf("Thread 1 spawned\n");	    
    printf("pthread_create : time before pthread_create : %ld ns\n", usage_before.tv_sec*1000000000 + usage_before.tv_nsec);
    printf("pthread_create : time after pthread_create : %ld ns\n", usage_after.tv_sec*1000000000 + usage_after.tv_nsec);
    total_time = (usage_after.tv_sec*1000000000 + usage_after.tv_nsec) - (usage_before.tv_sec*1000000000 + usage_before.tv_nsec); /* Total time in for pthread_create */
    printf("pthread_create : Total  time for pthread_create : %ld ns\n", total_time);
    if(pthread_join(tid_1, NULL) == 0)  /* Waits for child thread to complete its execution */
       printf("Thread 1 done\n");
    else
       perror("Thread 1");  


/********************** fork() and exec() **************************************************/         
    if(clock_gettime(CLOCK_THREAD_CPUTIME_ID,&usage_before))           /* Get the current time */
        printf("ERROR:clock_gettime before fork \n");
	pid = fork();   
    if(pid)
	{
	   //printf("******* In Parent Process *******\n");
	   int status = 0;
	   //printf("Fork returned value is %d\n", pid);
	   if(waitpid(pid, &status, 0) == -1)          /* waits for chils process to complete its execution */
			perror("wait error");
	   if(clock_gettime(CLOCK_THREAD_CPUTIME_ID,&usage_after))           /* Get the current time */
                printf("ERROR:clock_gettime after exec \n");
        printf("fork and exec : time before fork : %ld ns\n", usage_before.tv_sec*1000000000 + usage_before.tv_nsec);
        printf("fork and exec : time after exec : %ld ns\n", usage_after.tv_sec*1000000000 + usage_after.tv_nsec);
        total_time = (usage_after.tv_sec*1000000000 + usage_after.tv_nsec) - (usage_before.tv_sec*1000000000 + usage_before.tv_nsec); /* Total time in for pthread_create */
        printf("fork and exec : Total  time for fork and exec : %ld ns\n", total_time);
	}
	else if(pid == 0)
	{ 
		printf("******* In Child Process *******\n");
		static char *argv[]={"test", "0", NULL};
		//static char *argv[]={NULL};
		execv("exit", argv);            /* Exiting exit(0) */
		exit(1);
	}
	else 
	    perror("forking error");	
	return;


}
