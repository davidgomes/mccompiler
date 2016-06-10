
char buf[15];

int f(void) {
  return 5;
}

int g(void) {
  int k;
  int b;
  int c;
  b = 4;
  c = 5;
  k = c && b;
  return k;
}

int main(void) {
  puts(itoa(f() && g(), buf));
}