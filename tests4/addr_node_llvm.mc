int f(void) {
  return 10;
}

int* v(void){
  int c;

  return &c;
}

int p(int *c){
  return 0;
}


int main(void){
  char a[10];
  void *k;
  int *b;
  int c;

  b = &c;
  k = &a;
  p(&c);

  return 0;
}
