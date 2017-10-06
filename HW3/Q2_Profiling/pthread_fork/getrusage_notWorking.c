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
#include <sys/resource.h>
#include <sys/wait.h>
#include <pthread.h>
#include <stdlib.h>

#define FORK_EXEC	 1
#define PTHREAD_CREATE    0
#define KTHREAD_CREATE    0

/* Thread start routine */
void *dummy_fn(void *threadid)
{
    printf("Do Nothing in thread function\n");
    pthread_exit(NULL);
}

void main()
{
	struct rusage usage_before;
	struct rusage usage_after;
	size_t user_time;
	size_t kern_time;
	pid_t pid;
	pthread_t tid_1;
    pthread_attr_t t1_attr;
    size_t rc;
 
 /********************** Pthread Create **************************************************/ 
  
    pthread_attr_init(&t1_attr);            /* initializes the pthread attribute values */
    getrusage(RUSAGE_THREAD, &usage_before);  /* Gets the resource usage in user and kernel space before pthread_create */
    rc = pthread_create(&tid_1, &t1_attr, dummy_fn, NULL);  /* Call to create a new thread */
    getrusage(RUSAGE_THREAD, &usage_after);   /* Gets the resource usage in user and kernel space after pthread_create */
    if (rc) 
    {
        printf("ERROR; pthread_create() rc is %ld\n", rc); 
        perror(NULL); exit(-1);
    }
    printf("Thread 1 spawned\n");	    
    printf("pthread_create : user cpu time before pthread_create : %ld us\n", usage_before.ru_utime.tv_sec*1000000 + usage_before.ru_utime.tv_usec);
    printf("pthread_create : kern cpu time before pthread_create : %ld us\n", usage_before.ru_stime.tv_sec*1000000 + usage_before.ru_stime.tv_usec);

    printf("pthread_create : user cpu time after pthread_create : %ld us\n", usage_after.ru_utime.tv_sec*1000000 + usage_after.ru_utime.tv_usec);
    printf("pthread_create : kern cpu time after pthread_create : %ld us\n", usage_after.ru_stime.tv_sec*1000000 + usage_after.ru_stime.tv_usec);
    user_time = (usage_after.ru_utime.tv_sec*1000000 + usage_after.ru_utime.tv_usec) - (usage_before.ru_utime.tv_sec*1000000 + usage_before.ru_utime.tv_usec); /* Total CPU time in user space */
    kern_time = (usage_after.ru_stime.tv_sec*1000000 + usage_after.ru_stime.tv_usec)-(usage_before.ru_stime.tv_sec*1000000 + usage_before.ru_stime.tv_usec);   /* Total CPU time in kernel space */
    printf("pthread_create : Total user time for fork : %ld us\n", user_time);
    printf("pthread_create : Total kern time for fork : %ld us\n", kern_time);  
    if(pthread_join(tid_1, NULL) == 0)  /* Waits for child thread to complete its execution */
       printf("Thread 1 done\n");
    else
       perror("Thread 1");  


/********************** fork() and exec() **************************************************/         
    getrusage(RUSAGE_SELF, &usage_before);      /* Gets the resource usage in user and kernel space before fork */
	pid = fork();   
    if(pid)
	{
	   //printf("******* In Parent Process *******\n");
	   int status = 0;
	   //printf("Fork returned value is %d\n", pid);
	   if(waitpid(pid, &status, 0) == -1)          /* waits for chils process to complete its execution */
			perror("wait error");
	   getrusage(RUSAGE_SELF, &usage_after);    /* Gets the resource usage in user and kernel space after fork */
       printf("PARENT : user cpu time before fork : %ld us\n", usage_before.ru_utime.tv_sec*1000000 + usage_before.ru_utime.tv_usec);
       printf("PARENT : kern cpu time before fork : %ld us\n", usage_before.ru_stime.tv_sec*1000000 + usage_before.ru_stime.tv_usec);
       printf("PARENT : user cpu time after exec : %ld us\n", usage_after.ru_utime.tv_sec*1000000 + usage_after.ru_utime.tv_usec);
       printf("CHILD : kern cpu time after exec: %ld us\n", usage_after.ru_stime.tv_sec*1000000 + usage_after.ru_stime.tv_usec);
       user_time = (usage_after.ru_utime.tv_sec*1000000 + usage_after.ru_utime.tv_usec) - (usage_before.ru_utime.tv_sec*1000000 + usage_before.ru_utime.tv_usec); /* Total CPU time in user space */
       kern_time = (usage_after.ru_stime.tv_sec*1000000 + usage_after.ru_stime.tv_usec) - (usage_before.ru_stime.tv_sec*1000000 + usage_before.ru_stime.tv_usec); /* Total CPU time in kernel space */
       printf("PARENT : Total user time for fork and exec : %ld us\n", user_time);
       printf("PARENT : Total kern time for fork and exec  %ld us\n", kern_time);
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
	    while(1);
	return;


}
