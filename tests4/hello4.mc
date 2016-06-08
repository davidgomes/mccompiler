char buf[15];
int arr[20];

int main(void) {
  int *p, *p2, b;

  p = arr + 1;
  p2 = arr + 1;

  b = p2 != p;

  puts(itoa(b, buf));
}