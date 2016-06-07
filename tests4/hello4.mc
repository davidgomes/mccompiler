char buf[15];

int main(int argc, char **argv) {
  int a;
  int *a2;
  int **a3;
  int ***a4;

  a2 = &a;
  a3 = &a2;
  a4 = &a3;

  ***a4 = 5;

  puts(itoa(a, buf));

  if (a == 7) {
    return 7;
    puts("wat");
  }

  return 0;

  puts("hello");
}
