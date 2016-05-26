int a;
char buf[20];

char *itoa(int value, char *buf) {
  sprintf(buf, "%d", value);
  return buf;
}

void v(void) {
  a = 7;
}

int main(void) {
  int a;

  a = 5;
  v();

  itoa(a, buf);

  puts(buf);

  return 0;
}