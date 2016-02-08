#include <pthread.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#define NUM_THREADS 6
#define INITAL_APPROX 3
#define PI_APPROX 4

pthread_t thread[NUM_THREADS];
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
double result;

typedef struct calc_data
{
  int n;
} calc_data;

int isOdd(int n)
{
  if(n%2)
    return 1;
  else
    return -1;
}

void* nilakantha_series(void* arg)
{
  //pi = 3 + 4/(2*3*4) - 4/(4*5*6) + 4/(6*7*8) - ...
  
  calc_data *c_data = (calc_data*) arg;
  int n = c_data->n;
  double approx;
  
  if(n + 1 == 1)
  {
    approx = INITAL_APPROX;
  }
  else if(n + 1 == 2)
  {
    approx = (PI_APPROX / (double) ((2*n) * ((2*n)+1) * ((2*n)+2)));
  }
  else
  {
    approx = (PI_APPROX / (double) ((2*n) * ((2*n)+1) * ((2*n)+2)) * isOdd(n));
  }
  
  printf("in result for n=%d is %f\n", n, approx);
  pthread_mutex_lock(&lock);
  result += approx;
  pthread_mutex_unlock(&lock);
  pthread_exit(NULL);
}

int main(void)
{
  int t, rc;
  calc_data c_data;
  
  for(t=0; t<NUM_THREADS; t++)
  {
    c_data.n = t;    
    printf("\nCreating thread #%d\n", t);
    rc = pthread_create(&thread[t], NULL, nilakantha_series, (void*) &c_data);  
    usleep(1000);
    if(rc)
    {
      printf("error in thread create with status %d\n", rc);
      return -1;
    }   
  }
  
  printf("\n");
  
  for(t=0; t<NUM_THREADS; t++)
  {
    pthread_join(thread[t], NULL);
    printf("joining thread #%d\n", t);
  }
  
  pthread_mutex_destroy(&lock);
  printf("\npi approximation: %f\n", result);
  return 0;
}
