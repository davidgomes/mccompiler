#include <stdio.h>

void *v1, **v2, ***v3;
char *c1;
int *i1, i, **i2;


int main(void) {
  v2 = v1;
  v3 = v1;
}