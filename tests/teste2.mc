int b[10];

void f(int, char);

void f2(char **b) {
  int c[20];
  b = f;
  c = f;
}