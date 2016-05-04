void z(void) {

}

char buffer2[30];

int f(void) {
  return 10;
}

int puts(void) {
  void *mm;
  char buffer[20];
  mm = &f();
  mm = &buffer[2];
  mm = &buffer;
  mm = &buffer2;
}

void main(void) {
  int a;
  void *k;
  k = &a;
  k = &z;
  k = &(5 + 4);
}

int a(int, char);

int a(int k, char v) {
  char *o;
  o = &a;
}