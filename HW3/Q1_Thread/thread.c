/**
 * @file 
 *		thread.c
 * @brief 
 *		This file implements the different Pthread functionalities 
 *
 * @author 
 *		Saritha Senguttuvan
 * @date 
 *		Oct,2017
 *
 */

#define _GNU_SOURCE
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/* Thread names */
pthread_t tid_1;
pthread_t tid_2;

/* Mutex resource */
pthread_mutex_t rsrcA;

/* condition variable */
pthread_cond_t condA;

/* Pthread attributes */
pthread_attr_t main_attr;
pthread_attr_t t1_attr;
pthread_attr_t t2_attr;

int max_prio, min_prio;

struct sched_param main_param;
struct sched_param t1_param;
struct sched_param t2_param;
pid_t mainpid;

size_t sum = 0;

/* Thread 1 start function */
void *dummy_fn(void *threadid)
{
   int rc = 0;
   size_t i = 0;
   size_t num = (size_t)threadid;
   size_t stack_size, guard_size;

   void *stack_addr;
   printf("In the thread 1 function\n");
   printf("Thread 1 trying to grab resource\n");
   rc = pthread_mutex_lock(&rsrcA);	                    /* Thread 1 trying to acquire the mutex lock */
   if(rc)
        printf("thread1: pthread_mutex_lock error\n");   
   printf("Thread 1 got the resource\n");
   printf("The thread id from the thread 1 function is %ld\n", pthread_self());         /* Returns the ID of the calling thread */ 

   rc = pthread_getattr_default_np(&t1_attr);                   /* Gets the default attribute values of the thread */
   if(rc != 0)
        printf("ERROR; pthread_getattr_default_np() rc is %d\n", rc);  
      
   rc = pthread_attr_getguardsize(&t1_attr, &guard_size);       /* Gets the guard size for the current thread */
   if (rc  != 0)
        printf("ERROR: pthread_attr_getguardsize() rc is %d\n", rc); 
   printf("The default guard size of thread 1 is %ld bytes\n", guard_size);     

   rc = pthread_attr_getstack(&t1_attr, &stack_addr, &stack_size);      /* Gets the stack address and stack value for the current thread */
   if (rc != 0)
        printf("ERROR; pthread_attr_getstack() rc is %d\n", rc); 
   if (stack_size > 0)
        printf("The default stack size is %ld bytes\n", stack_size);       
   rc = pthread_getattr_np(tid_1,&t1_attr);                     /* Get the attributes of the current thread */
   if(rc != 0)
        printf("ERROR; getattr_np() rc is %d\n", rc);  
      
   rc = pthread_attr_getguardsize(&t1_attr, &guard_size);       
   if (rc  != 0)
        printf("ERROR: pthread_attr_getguardsize() rc is %d\n", rc); 
    printf("The new guard size of thread 1 is %ld bytes\n", guard_size);     

    rc = pthread_attr_getstack(&t1_attr, &stack_addr, &stack_size);
    if (rc != 0)
        printf("ERROR; pthread_attr_getstack() rc is %d\n", rc); 
    if (stack_size > 0)
        printf("The new stack size is %ld bytes\n", stack_size);
   
   usleep(1000000);
   
   /*   
   for(i=1; i<=(num); i++)
       sum = sum + i;
   printf("The sum value in the thread 1 function is %ld\n", sum);
   rc = pthread_mutex_unlock(&rsrcA); 
   if(rc)
        printf("Thread1 pthread_mutex_unlock error\n");    
   printf("Thread 1 released the resource\n");
   */
           
   for(i=1; i<=(num); i++)
       sum = sum + i;                                   /* Computes the sum value */
   rc = pthread_cond_signal(&condA);                    /* Thread 1 sends the condition signal */
   if (rc) 
   {
       printf("ERROR; pthread_cond_signal() rc is %d\n", rc); 
       perror(NULL); 
       exit(-1);
   }   
   printf("The condition from thread 1 has reached\n"); 
   rc = pthread_mutex_unlock(&rsrcA);                   /* Releases the mutex lock */
   if(rc)
        printf("Thread1 pthread_mutex_unlock error\n");  
   printf("Thread 1 released the resource\n");
   pthread_exit(NULL);   
}

/* Threead 2 function */
void *dummy2_fn(void *threadid)
{
   size_t rc = 0;
   printf("Thread 2 trying to grab the resource\n");
   size_t i = 0;
   rc = pthread_mutex_lock(&rsrcA);                     /* Thread 2 trying to acquire the mutex lock */
   if(rc)
        printf("Thread2: pthread_mutex_lock error %ld\n",rc);	
   printf("Thread 2 got the resource\n");    
   /*
   //rc = pthread_mutex_trylock(&rsrcA);                /* Thread 2 trying to acquire the mutex lock using trylock */
   if(rc)
        printf("Thread2: pthread_mutex_trylock error %ld\n",rc);	
   printf("Thread 2 got the resource\n"); 
   size_t num = (size_t)threadid;
   printf("In the thread 2 function\n"); 
   printf("The thread id from the thread 2 function is %ld\n", pthread_self());
   
  /*  
   for(i=1; i<=(num); i++)
       sum = sum + i;
   printf("The sum value in the thread 2 function is %ld\n", sum); 
   */
   
   while(sum==0)
   {
        printf("The condition sum value is %ld. Using condition wait, control to thread 1 given\n", sum);
        rc = pthread_cond_wait(&condA, &rsrcA);                      /* Pthread waits for a condition, and releases the mutex until condition ha met */
        if(rc)
            printf("Thread2 pthread_cond_wait error\n");
   }
   printf("The sum value is %ld\n",sum);                             /* Displays the sum value */  
   rc = pthread_mutex_unlock(&rsrcA);                                /* Releases the mutex lock */
   if(rc)
        printf("Thread2 pthread_mutex_unlock error\n"); 
   printf("Thread 2 released the resource\n"); 
   pthread_exit(NULL);
}


int main ()
{
   size_t rc;
   size_t guard_size; 
   size_t input_var = 5;  
   size_t new_guard_size = 8192;   
   rc = pthread_mutex_init(&rsrcA, NULL);        /* Set default protocol for mutex */
   if(rc)
      printf("Mutex init error\n");
   rc = pthread_cond_init(&condA, NULL);           /* initializing the mutex condition variable */
   if(rc)
      printf("condition init error\n");
   pthread_attr_init(&t1_attr);                     /* initialzing thread1 attributes */
   pthread_attr_init(&t2_attr);                      /* initialzing thread2 attributes */
   printf("The thread id from the main function is %ld\n", pthread_self());
   mainpid=getpid(); 
   rc=sched_getparam(mainpid, &main_param); 
   if(rc < 0) 
        printf("ERROR: sched_getparam");
   rc=sched_setscheduler(getpid(), SCHED_FIFO, &main_param);
   if(rc < 0) 
        printf("ERROR: sched_setscheduler");

   max_prio = sched_get_priority_max(SCHED_FIFO);
   min_prio = sched_get_priority_min(SCHED_FIFO);
   main_param.sched_priority=max_prio; 

   rc=sched_setparam(mainpid, &main_param); 
   if(rc < 0) 
        printf("ERROR: sched_setparam");         
   rc = pthread_setschedprio(pthread_self(), max_prio);
   if(rc < 0) 
        printf("ERROR: pthread_setschedprio");

   if (new_guard_size >= 0) 
        rc = pthread_attr_setguardsize(&t1_attr, new_guard_size);   /* Set the guard size for the thread */     
   if (rc != 0)
        printf("ERROR; pthread_attr_setguardsize() rc is %ld\n", rc); 


   rc = pthread_create(&tid_1, &t1_attr, dummy_fn, (void *)input_var);       /* creating a new thread*/
   if (rc) 
   {
        printf("ERROR; pthread_create() rc is %ld\n", rc); 
        perror(NULL); exit(-1);
   }
   printf("Thread 1 spawned\n");

   rc = pthread_create(&tid_2, &t2_attr, dummy2_fn, NULL);          /* creating a new thread*/
   if (rc) 
   {
        printf("ERROR; pthread_create() 2 rc is %ld\n", rc); 
        perror(NULL); exit(-1);
   }
   printf("Thread 2 spawned\n");
   
   printf("Control in main\n");
   
   if(pthread_join(tid_1, NULL) == 0)   /* Parent waits for the child thread1 to complete it's execution */
       printf("Thread 1 done\n");
   else
       perror("Thread 1");  
       
   if(pthread_join(tid_2, NULL) == 0)  /* Parent waits for the child thread2 to complete it's execution */
       printf("Thread 2 done\n");
   else
       perror("Thread 2"); 
      
   rc = pthread_mutex_destroy(&rsrcA);              /* Destroy the mutex */
   if(rc)
        printf("Mutex destroy error: %ld\n", rc);
   printf("Mutex destroyed\n");
   rc = pthread_cond_destroy(&condA);       /* Destory the condition variable */
   if (rc) 
   {
        printf("ERROR; pthread_cond_destroy() rc is %ld\n", rc); 
        perror(NULL); 
        exit(-1);
   }
   return 0;
   
}
