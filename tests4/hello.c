int main(int ac, char **av) {
  char buffer[20];

  int c;
  int a;

  a = 5;
  c = 'a';

  if (c && a) {
    printf("%d\n", a && c);
  }

  return 0;
}