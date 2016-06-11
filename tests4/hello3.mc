char buf[15];

int *arr2[20];
int **i1, **i2;

int main(void) {
  i1 = arr2;
  i2 = arr2 + 1;

  puts(itoa(i2 - i1, buf));

  return 0;
}
