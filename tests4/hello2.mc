char buf[15];

int main(void) {
  int a, b, c;
  b = 1;
  c = 1;

  if ((b && c && 0) || 0) {
    a = (b && (c && 1)) || 0;
    puts(itoa(a, buf));
  }
}

