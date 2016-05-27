void v(void) {
}

void f(void) {
  return v();
}

int calc(int a, int b) {
  return 0;
}

int main(int argc, char **argv) {
  /*void **v2;*/
  char *buf;
  void *v1;
  int *i1;

  v1 = i1;
  i1 = v1;
  v1 = buf;
  buf = v1;

  itoa(5, buf);
  f();
  puts(buf);

  return 0;
}