/*int f(int k, int k2) {
  puts("hey");
  return 2;
}

int main(int argc) {
  int a;
  char c;
  a = 4;
  c = 'h';
  puts("hey");
  puts("he");
  return 1;

  int b;
  char c;
  char *buf;
  b = f(5, 7);
  buf = itoa(b, buf);

  puts();

  return 0;
}*/

int g(int k, int k2);
int l(void);

int l(void) {
  return 1;
}

int g(int v, int v2) {
  return v;
}

int f(int k, int k2) {
  return g(k, k2);
  /*l();
  return l();*/
}

int main(int argc) {
  int a;
  int b;
  char *buf;

  a = b;
  a = 0;
  a = f(0, 4);

  return a;
}