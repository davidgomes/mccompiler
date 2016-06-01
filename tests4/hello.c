#include <stdio.h>

int main(int ac, char **av) {
  int i = 4;
  int *i1 = &i;

  printf("%d\n", *i1);

  return 0;
}