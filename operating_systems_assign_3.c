/**************************************************************************************************
*  John Santoro (U87402138)
*  COP4600.001F16
*  Description:  Reads a file.  This program uses a 'producer' thread to add characters
*                from the file it is reading to a buffer, and then a 'consumer' thread to
*                read characters from that said buffer.  Semaphores are used to ensure that
*                the buffer can contain no more than 15 characters at a time.  In order to
*                show that the semaphores are indeed working properly, a 1-second delay is
*                used between reads in the consumer thread.
**************************************************************************************************/

#define _REENTRANT

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <semaphore.h>
#include <unistd.h>

// Shared Memory -----------------------------------------
/* change the key number */
#define SHMKEY ((key_t) 7878)

#define BUFFER_SIZE 15
#define MAX_CHAR_COUNT 150

typedef struct  // Buffer and its length are stored in this structure.
{
  char buffer[BUFFER_SIZE];
  int length;
  int consumerPos;
  int eofFlag;
} shared_mem;

shared_mem* shared_buffer;

/* Semaphore Declaration ------------------------------------------- */
sem_t mutex;
sem_t full;
sem_t empty;

// Thread Functions -----------------------------------------------
void* producer()
{
  int charCounter = 0;
  int endOfFile = 0;
  shared_buffer->length = 0;
  
  char newChar;
  FILE* fp;
  fp= fopen("mytest.dat", "r");
  
  while (charCounter < MAX_CHAR_COUNT)
  {
    // Item already produced; simply increment charCounter.
    charCounter++;
    endOfFile = (fscanf(fp, "%c", &newChar) == EOF);
  
    if (endOfFile == 0)
    {
      sem_wait(&empty);
      sem_wait(&mutex);
      
      // Add item to buffer.  Modulus added to prevent overflow.
      shared_buffer->buffer[(shared_buffer->length++) % BUFFER_SIZE] = newChar;
      
      // Print at each step.
      // printf("Producer:  %c --> len = %d, pos = %d\n", newChar, shared_buffer->length, shared_buffer->consumerPos);
    
      sem_post(&mutex);
      sem_post(&full);
    }
    else
    {
      sem_wait(&empty);
      sem_wait(&mutex);
      
      shared_buffer->eofFlag = 1;
      shared_buffer->buffer[(shared_buffer->length) % BUFFER_SIZE] = '*';
      // Print at each step.
      // printf("Producer:  END OF FILE\n");
      
      sem_post(&mutex);
      sem_post(&full);
      
      break;
    }
  }

  close(fp);
  return NULL;
}

void* consumer()
{
  // int i;
  char nextC;
  shared_buffer->consumerPos = 0;
  
  do
  {
    fflush(stdout);
  
    sem_wait(&full);
    sem_wait(&mutex);
    
    // Remove item from buffer and set to next character.
    sleep(1);  // Consumer consumes more slowly than producer produces.
    
    nextC = shared_buffer->buffer[(shared_buffer->consumerPos++) % BUFFER_SIZE];
      
    // Print at each step.
    // printf("Consumer:  %c --> len = %d, pos = %d\n", nextC, shared_buffer->length, shared_buffer->consumerPos);
    
    sem_post(&mutex);
    sem_post(&empty);
    
    // Print item, checking for EOF character.
    if (nextC != '*')
      printf("%c", nextC);
  }
  while (shared_buffer->eofFlag == 0 || shared_buffer->consumerPos < shared_buffer->length);

  printf("\nFile read.\n");
  return NULL;
}

// Main --------------------------------------------------
int main(void)
{
  int shmid;              /* Shared Memory ID */
  pthread_t tid1[1];      /* Process ID for Thread 1 */
  pthread_t tid2[1];      /* Process ID for Thread 2 */
  pthread_attr_t attr[1]; /* Attribute Pointer Array */
  
  /* Semaphore Initialiation ----------------------------------------- */

  if (sem_init(&mutex, 1, 1) == -1) // Initialize mutex to 1
  {
    printf("Semaphore 'mutex' failed to initialize.");
    exit (1);
  }
  
  if (sem_init(&full, 1, 0) == -1) // Initialize full to 0
  {
    printf("Semaphore 'full' failed to initialize.");
    exit (1);
  }
  
  if (sem_init(&empty, 1, BUFFER_SIZE) == -1) // Initialize empty to BUFFER_SIZE
  {
    printf("Semaphore 'empty' failed to initialize.");
    exit (1);
  }
  
  fflush(stdout);
  
  /* Create and initialize shared memory ----------------------------- */
  char *shmadd;
  shmadd = (char *) 0;
  
  // Shared memory size should be set according to the fact that it contains an array of 15 characters and 3 integers.
  if ((shmid = shmget (SHMKEY, sizeof(char) * 15 + sizeof(int) * 3, IPC_CREAT | 0666)) < 0)  
  {
    perror ("shmget");
    exit (1);
  }

  if ((shared_buffer = (shared_mem *) shmat (shmid, shmadd, 0)) == (shared_mem *) -1)
  {
    perror ("shmat");
    exit (0);
  }
  
  // counter->value = 0;
  
  /* Required to schedule thread independently. ---------------------- */
  pthread_attr_init(&attr[0]);
  pthread_attr_setscope(&attr[0], PTHREAD_SCOPE_SYSTEM);
  /* End to schedule thread independently. */
  
  /* Create the threads */
  pthread_create(&tid1[0], &attr[0], producer, NULL);
  pthread_create(&tid2[0], &attr[0], consumer, NULL);
  
  /* Wait for the threads to finish */
  pthread_join(tid1[0], NULL);
  pthread_join(tid2[0], NULL);
  
  // printf("From Parent Counter = %s\n", shared_buffer->buffer);
  printf("------------------------------------\n");
  printf("\t\tEnd of simulation\n");
  
  /* Free shared memory -------------------------------------------------- */
  // Attempt to release shared memory
  if ((shmctl(shmid, IPC_RMID, (struct shmid_ds *) 0)) == -1)
  {
    perror ("shmctl");
    exit (-1);
  }
  
  /* Free semaphores ------------------------------------------------------ */
  if (sem_destroy(&mutex) == -1)
  {
    printf("Error destroying semaphore 'mutex'.");
    exit (1);
  }
  
  if (sem_destroy(&full) == -1)
  {
    printf("Error destroying semaphore 'full'.");
    exit (1);
  }
  
  if (sem_destroy(&empty) == -1)
  {
    printf("Error destroying semaphore 'empty'.");
    exit (1);
  }
  
  exit(0);
}