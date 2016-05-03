int i, **i2;
void *v1, **v2;
void *v1_2;

void *main(void) {
  v1 = i2;
  v2 = v1;
  i = 7;
  i = '7';

  v1 = v1_2;
}