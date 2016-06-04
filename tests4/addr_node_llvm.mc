int f(void) {
  return 10;
}

int* v(void){
  int c;

  return &c;
}

void p(int *c){

}


int main(void){
  int a;
  void *k;
  int *b;
  int c;

  b = &c;
  k = &a;
  p(&c);

  return 0;
}
