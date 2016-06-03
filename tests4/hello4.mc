int cenas(void) {
  int a;
  int b;
  int arr[20];

  arr[0] = 3;

  a = 6;

  return a >= arr[0] + 4;
}

int main(int ac, char **av) {
  char buffer[20];

  itoa(cenas(), buffer);

  puts(buffer);

  return 0;
}