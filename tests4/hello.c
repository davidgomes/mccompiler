int factorial(int n) {
  if (n == 1) {
    return 1;
  } else {
    return n * factorial(n - 1);
  }
}

int main(int ac, char **av) {
  char buffer[20];
  int res;

  res = factorial(5);
  itoa(res, buffer);

  puts(buffer);

  return 0;
}