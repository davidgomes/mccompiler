#include <stdio.h>
#include <string.h>

char b;

int mystrlen(const char *str)
{
  int val = 0;

  int i = 0;

  while (i < strlen(str)) {
    printf("%c\n", str[i]);

    if (str[i] == '\\') {
      printf("here\n");
      i += 2;
    } else {
      val++;
      i += 1;
    }
  }

  return val;
}

int main(void) {
  char *tst = "hey\n\t";

  printf("%d\n", mystrlen(tst));
  printf("%d\n", (int) strlen(tst));

  return 0;
}