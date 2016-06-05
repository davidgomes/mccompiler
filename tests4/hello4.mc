char buf[15];

int main(int argc, char **argv) {
  int a, b, c;

  a = 1;
  b = 0;
  /*c = (b=b+1) || (a=0);*/

  a = 0 && (b = 1);

  puts(itoa(a, buf));
  puts(itoa(b, buf));

  return 0;
}
