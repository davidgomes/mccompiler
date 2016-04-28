void z(void) {

}

int f(void) {
  return 10;
}

int puts(void) {
  void *mm;
  mm = &f();
}

void main(void) {
  int a;
  void *k;
  k = &a;
  k = &z;
}

int a(int, char);

int a(int k, char v) {
  char *o;
  o = &a;
}