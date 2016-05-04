#include <stdio.h>

void v(void) {}

char* g(void){}

int main(void) {
  /*void *v1, **v2, ***v3;

  v1 = v2;
  v2 = v1;

  v3 = v1;

  v1[4];*/

  /*for (v();v();v()) {

  }*/

  &g();
}