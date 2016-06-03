int main(int ac, char **av) {
  char buffer[20];

  int c;
  int a;

  a = 5;
  c = 'a';

  if (5) {
    itoa('a' || '\0', buffer);
    puts(buffer);
  }

  return 0;
}