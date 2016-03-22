char buffer[20];

int main(int argc, char **argv) {
  int a, b;
  a = atoi(argv[1]);
  b = atoi(argv[2]);

  if (a == 0) {
    puts(itoa(b, buffer));
  } else {
    for ( ; b > 0 ; )
      if (a > b)
        a = a-b;
      else
        b = b-a;
    puts(itoa(a, buffer));
  }
  return 0;
}

int f(void, int) {

}
