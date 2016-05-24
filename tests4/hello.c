#include <stdio.h>
#include <stdlib.h>

int f(int k, int k2) {
  puts("hey");
  return 2;
}

int main(void) {
  /*int a;
  char c;
  a = 4;
  c = 'h';
  puts("hey");
  puts("he");
  return 1;*/

  int b;
  char *buf;
  b = f(5, 7);

  /*puts();*/

  return 0;
}
