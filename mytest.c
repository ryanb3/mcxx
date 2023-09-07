#include <stdio.h>
#include <omp.h>

int main(int argc, char *argv[]) {

  #pragma omp nondeter

  #pragma omp parallelnew num_threads(3)
  {
    int test = 42;
    printf("Value of test: %d\n", test);
  }

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
