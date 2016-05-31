int factorial(int n) {
  int val;

  if (n == 1) {
    val = 1;
  } else {
    val = n * factorial(n - 1);
  }

  return val;
}

int main(int ac, char **av) {
  char buffer[20];
  int res;

  res = factorial(5);
  itoa(res, buffer);

  puts(buffer);

  return 0;
}