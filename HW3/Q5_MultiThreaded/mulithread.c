#define _GNU_SOURCE
#include <stdio.h>
#include <signal.h>
#include <pthread.h>
#include <stdlib.h>
#include <semaphore.h>

struct sigaction my_sig;

/* Semaphore names */
sem_t sem1;
sem_t sem2;

/* Mutex name */
pthread_mutex_t rsrcA;

FILE *file_ptr;     /* Pointer pointing to the file */

/* Thread names */
pthread_t tid_2;
pthread_t tid_3;

/* Structure to hold the word count, line count , characters */
typedef struct{
	size_t words;
	size_t lines;
	size_t characters;
}file_data;

file_data statistics;

/* Defining my owm handler to handle the system signals */
void my_handler(int signum)
{
    printf("AES\n");
    printf("Handler id is %ld\n",pthread_self());
    if (signum == SIGUSR1)                  /* If SIGUSR1 signal */
    {
        printf("Received SIGUSR1!\n");
        sem_post(&sem1);                    /* Post semaphore to thread2 */
    }
    if (signum == SIGUSR2)                  /* If SIGUSR2 signal */
    {
        printf("Received SIGUSR2!\n");      /* Post semaphore to thread3*/
        sem_post(&sem2);
    }
    if(signum == SIGTERM)                   /* If SIGTERM signal */
    {
        printf("Received SIGTERM!\n");
        if(file_ptr != NULL)
            fclose(file_ptr);               /* closing any files that was left opened */
       
        pthread_kill(tid_2, SIGTERM);       /* Kill the thread 2 with pthread kill */
        pthread_kill(tid_3, SIGTERM);       /* Kill the thread 3 with pthread kill */
        
        pthread_mutex_destroy(&rsrcA);      /* Destroy the mutex */ 
        sem_destroy(&sem1);                 /* Destroy sem1 */
        sem_destroy(&sem2);                 /* Destroy sem2 */
       
        printf("Exited Normally \n");
        exit(0);  
    }
}

/* start routine for thread2 */
void *thread2_fn(void *threadid)
{
    FILE *t2_fptr;
    char c;    
    printf("Thread2 waiting for semaphore 1\n");
    sem_wait(&sem1);                            /* Waiting for semaphore from signal handler */
    printf("Thread2 got semaphore 1\n");
    printf("Thread2 trying to get mutex lock\n");
    pthread_mutex_lock(&rsrcA);                 /* thread 2 acquiring the mutex lock */
    printf("Thread2 got mutex lock\n");    
    t2_fptr = fopen("multithread", "r");        /* Open a file and return the address */
    printf("Data processing\n");
    if (t2_fptr)
    {
	   while ((c=getc(t2_fptr)) != EOF) 
	   {
		    if (c!= ' ' && c!= '\n')            /* number of characters increased  excluding new line or space */
		    { 
		        statistics.characters++; 
		    }

		   if (c == ' ' || c == '\n') /* number of words increased if new line or space */
		   { 
		        statistics.words++; 
		   }  
		   if (c == '\n')             /* number of lines increased if new line character */
		   { 
		        statistics.lines++;  
		   }  
	   }
	   if(statistics.characters>0)
		  statistics.lines++;
		  statistics.words++; 
    }
    else
    {
         printf("Failed to open the file\n");
    }
    fclose(t2_fptr);            /* Close the file that was opened */
    printf("Thread2 releasing mutex lock\n");
    pthread_mutex_unlock(&rsrcA);   /* Release the mutex lock */
    pthread_exit(NULL);     
}

/* Start routine for thread 3 */
void *thread3_fn(void *threadid)
{
    printf("Thread3 waiting for semaphore 2\n");
    sem_wait(&sem2);            /* Waits for Sem post from signal handler */
    printf("Thread3 got semaphore 2\n");
    printf("Thread3 trying to get mutex lock\n");
    pthread_mutex_lock(&rsrcA);         /* acquiring the mutex lock */
    printf("Thread3 got mutex lock\n");
    printf("Lines : %ld \n", statistics.lines);
    printf("Words : %ld \n", statistics.words);
    printf("Characters : %ld \n", statistics.characters);
    printf("Thread3 releasing mutex lock\n");
    pthread_mutex_unlock(&rsrcA);       /* Releasing the mutex lock */
    pthread_exit(NULL);   
}

int main()
{
    int rc;
    size_t i = 0;
    char str[200];
    /* Thread atributes */
    pthread_attr_t t2_attr;
    pthread_attr_t t3_attr;
    
    /* Initialising the thread attributes */
    pthread_attr_init(&t2_attr); 
    pthread_attr_init(&t3_attr); 
    
    pthread_mutex_init(&rsrcA, NULL);  /* Initializes the mutex */
    sem_init(&sem1, 0, 0);  /* Initialize the semaphore */
    sem_init(&sem2, 0, 0);  /* Initialize the semaphore */  
    file_ptr = fopen("multithread", "a+"); /* Open the file with permissions */
    printf("Enter the text\n");

    scanf("%[^\t]%*c", str);
    fprintf(file_ptr, "%s", str);   
       
    fclose(file_ptr);       /* Close the file pointer */
    file_ptr = NULL;
    my_sig.sa_handler = my_handler; /* Assigning the signal handler function */
    sigaction(SIGUSR1, &my_sig, NULL);  /* Registering the signals */ 
    sigaction(SIGUSR2, &my_sig, NULL);   
    sigaction(SIGTERM, &my_sig, NULL);  
    rc = pthread_create(&tid_2, &t2_attr, thread2_fn, NULL);  /* Create thread2  */
    if (rc) 
    {
        printf("ERROR; pthread_create() 2 rc is %d\n", rc); 
        perror(NULL); 
        exit(-1);
    }
    printf("Thread 2 spawned\n");
    rc = pthread_create(&tid_3, &t3_attr, thread3_fn, NULL);  /* Create thread 3 */ 
    if (rc) 
    {
        printf("ERROR; pthread_create() 3 rc is %d\n", rc); 
        perror(NULL); 
        exit(-1);
    }
    printf("Thread 3 spawned\n");  
    
    if(pthread_join(tid_2, NULL) == 0)      /* Wait until child has completed execution*/
       printf("Thread 2 done\n");
    else
       perror("Thread 2");  
       
    if(pthread_join(tid_3, NULL) == 0)    /* Wait until child has completed execution*/
       printf("Thread 3 done\n");
    else
       perror("Thread 3");  
       
   rc = pthread_mutex_destroy(&rsrcA);      /* Destroy the mutex */
   if(rc)
        printf("Mutex destroy error: %d\n", rc);
   printf("Mutex destroyed\n");
   
   rc = sem_destroy(&sem1);             /* Destroy the Semaphore1 */
   if(rc)
        printf("Semaphore 1 destroy error: %d\n", rc);
   printf("Semaphore 1 destroyed\n");
   rc = sem_destroy(&sem2);             /* Destroy the Semaphore2 */
   if(rc)
        printf("Semaphore 2 destroy error: %d\n", rc);
   printf("Semaphore 2 destroyed\n");    
    return(0);

}
