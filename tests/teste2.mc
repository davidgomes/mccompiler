int i, **i2;
void *v1, **v2;
void *v1_2;
char c, *c1;

void v(void) {}

void *main(void) {
  v1 = v2;
  v1 = v1;
  v2 = v2;
  v1 = v();
}