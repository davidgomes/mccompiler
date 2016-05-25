void v(void) {
  return v();
}

void f(void) {
  return v();
}

int main(int arc, char **argv) {
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
  /*puts("hey");*/
  return 0;
}