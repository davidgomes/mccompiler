int main(int argc, char **argv) {
  int buf[10];
  char buf2[20];
  buf[0] = 5;

  itoa(buf[0], buf2);

  puts(buf2);

  return 0;
}