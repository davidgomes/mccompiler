char c0;

char *itoa(int n, char* buf) {
  sprintf(buf, "%d", n);
  return buf;
}

int main(int ac, char **av) {
  char buffer[10];
  int dude = 192;

  c0 = dude;

  itoa(c0, buffer);
  puts(buffer);

  return 0;
}