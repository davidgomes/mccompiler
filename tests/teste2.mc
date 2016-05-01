int f(char *t) {
  return 10;
}

void main(void) {
  void *c;
  f(c);
}