char buf[10];

int main(int argc, char **argv) {
  int arr[10];
  int *p;

  arr[0] = 2;
  arr[1] = 3;
  *(arr + 1 + 1) = 4;

  *arr = 1;

  puts(itoa(arr[0], buf));
  puts(itoa(arr[1], buf));
  puts(itoa(arr[2], buf));

  return 0;
}
