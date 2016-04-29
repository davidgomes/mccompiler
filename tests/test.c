#include <stdio.h>
#include <string.h>

char b;

int mystrlen(const char *str)
{
  int val = 0;
  int i = 0;

  while (i < strlen(str)) {
    if (str[i] == '\n') {
      i += 1;
    } else {
      val++;
      i += 1;
    }
  }

  return val;
}

int main(void) {
  char tst[20] = "\"\n\"";

  printf("%d\n", mystrlen(tst));
  printf("%d\n", (int) strlen(tst));

  return 0;
}