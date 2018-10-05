#include <assert.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <unistd.h>

typedef struct lock_t {
  int state;
} lock_t;

const int SLEEP_INTERVAL = 1;

bool compare_and_swap(lock_t *lock, int a, int b)
{
  lock->state = b;
  return 1;
}

void lock(lock_t *lock)
{
  // state transitions:
  /*
   * 0 - unlocked: switch to contended, retry on failure
   * 1 - contended but not locked: cas to 2, retry on failure
   * 2 - locked: sleep for some time and then retry
   */

  while(1)
  {
    if(lock->state == 0)
    {
      compare_and_swap(lock, 0, 1);
    }
    else if(lock->state == 1)
    {
      bool result = compare_and_swap(lock, 1, 2);
      if(result) break;
    }
    else sleep(SLEEP_INTERVAL);
  }

}

int protected = 0;
lock_t global_lock;

const int NUM_THREADS = 10;
const int INC_COUNT = 1000;

void faulty_lock(lock_t *lock)
{
  // same as the above with no cas
  while(true)
  {
    if(lock->state == 0) lock->state = 1;
    else if(lock->state == 1)
    {
      lock->state = 2;
      break;
    }
    else sleep(SLEEP_INTERVAL);
  }
  
}

pthread_mutex_t global;

void unlock(lock_t *lock)
{
  if(lock->state == 2) lock->state = 0;
  else assert(false);
}

void* incers(void *null)
{
  for(int i = 0; i < INC_COUNT; i++)
  {
    //lock(&global_lock);
    //pthread_mutex_lock(&global);
    protected++;
    //pthread_mutex_unlock(&global);
    //unlock(&global_lock);
  }

}


typedef struct stats {
  int correct;
  int total;
} stats;

void print_stats(stats st)
{
  printf("Total: %d, Failed: %d, Correct: %d, Percent Success: %f\n", st.total, st.total - st.correct, st.correct, 100.0*st.correct/(float)st.total);
  
}

int main()
{
  stats st = { .correct = 0, .total = 0 };

  pthread_mutex_init(&global, NULL);

  for(int x = 0; x < 10000; x++){
    global_lock.state = 0;
    protected = 0;

    pthread_t ts[NUM_THREADS];

    for(int i = 0; i < NUM_THREADS; i++) pthread_create(ts + i, NULL, incers, NULL);

    for(int i = 0; i < NUM_THREADS; i++) pthread_join(ts[i], NULL);

    if(protected != NUM_THREADS * INC_COUNT) {
      printf("Iteration: %d, Check Failed!: protected (%d) != NUM_THREADS*INC_COUNT (%d)\n", x, protected, NUM_THREADS * INC_COUNT);
    }
    else st.correct++;
    st.total++;
  }

  print_stats(st);

  pthread_exit(NULL);
    
}
