void v(void) {}

void main(void) {
  v() + v();
  v() + 5;
  5 + v();

  v() - v();
  v() - 5;
  5 - v();

  v - v;
  v - 5;
  5 - v;
}