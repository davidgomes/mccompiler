int main(int ac, char **av) {
  int a;
  char buffer[20];

  a = (5, 4);

  itoa(a, buffer);
  puts(buffer);

  return 0;
}