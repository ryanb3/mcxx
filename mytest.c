/* test.c */

#include <stdio.h>

int main(int argc, char *argv[]) {

  #pragma omp nondeter

  #pragma omp parallel num_threads(3)
  {
    int id = omp_get_thread_num();
    int data = id;
    int total = omp_get_num_threads();
    printf("Greetings from process %d out of %d with value %d\n", id, total, data);
  }

  printf("parallel section ends.\n");

  return 0;
}