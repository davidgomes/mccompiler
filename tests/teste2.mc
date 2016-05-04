void v(void) {}

void *main(void) {
  int a, b;

  &v;
  &(a + b);
}