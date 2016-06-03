int main(int ac, char **av) {
  char buffer[20];

  int c;
  int a;
  char k;
  k = 0;

  k = !k;

  itoa(k, buffer);

  puts(buffer);

  return 0;
}