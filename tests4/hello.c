char *itoa(int n, char* buf) {
  sprintf(buf, "%d", n);
  return buf;
}

int cenas(void) {
  int a;
  int b;

  a = 1;
  b = 3;

  return a / b;
}

int main(int ac, char **av) {
  char buffer[20];

  itoa(cenas(), buffer);

  puts(buffer);

  return 0;
}