int main(int ac, char **av) {
  int i, u;
  char buffer[20];

  u = 20;
  i = -u;

  itoa(i, buffer);
  puts(buffer);

  i = +u;

  itoa(i, buffer);
  puts(buffer);  

  return 0;
}