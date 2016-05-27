int main(int argc, char **argv) {
  char a;
  int k;
  char buffer[20];

  a = '%';
  k = a;

  itoa(a, buffer);

  a = k;

  itoa(k, buffer);
  puts(buffer);

  return 0;
}