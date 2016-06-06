char buf[10];

int f(void) {
  return 5 == 5;
}

void cenas(void) {

}

int main(int argc, char **argv) {
  char c;

  c = f() + 2;

  cenas();

  puts(itoa(c, buf));

  return 0;
}
