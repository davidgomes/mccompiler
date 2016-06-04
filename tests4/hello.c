int main(int ac, char **av) {
  char buffer[20];

  int k = 4;
  int **a = &k;

  printf("%d\n", *a);

  return 0;
}