char *itoa(int n, char* buf) {
  sprintf(buf, "%d", n);
  return buf;
}

int main(int ac, char **av) {
  char buffer[20];

  int c;
  int a;
  char k;
  k = 0;

  itoa(!k, buffer);

  puts(buffer);

  return 0;
}