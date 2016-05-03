int i, **i2;
void *v1, **v2;
void *v1_2;
char c;

void *main(void) {
  v1 = i2;
  v2 = v1;
  i = 7;
  i = '7';

  c = '8';

  v1 = v1_2;
}