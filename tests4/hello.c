char *itoa(int n, char* buf) {
  sprintf(buf, "%d", n);
  return buf;
}

int main(int ac, char **av) {
  char buffer[20];

  int a = 5;
  int c = !a;

  itoa(c, buffer);

  puts(buffer);

  return 0;
}