char buf[15];

char cenas(char c) {
  return c + 1;
}

int main(int argc, char **argv) {
  int a;
  a = cenas(4);

  puts(itoa(a, buf));

  return 0;
}