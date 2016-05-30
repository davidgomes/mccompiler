char c0;

int main(int ac, char **av) {
  char buffer[10];

  c0 = 192;

  itoa(c0, buffer);
  puts(buffer);

  return 0;
}