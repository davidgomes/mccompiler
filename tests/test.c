#include <stdio.h>

void v(void) {

}

int main(void) {
  int buf[20];
  buf[0] = 20;

  printf("%d\n", *buf);
}