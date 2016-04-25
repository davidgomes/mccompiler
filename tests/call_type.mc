int f(int b) {
  return b + 1;
}

void v(void) {}

void main(void) {
  int c;
  int *b;
  c = f(v()) + 2;
  c = f(b);
  c = f(4);
}