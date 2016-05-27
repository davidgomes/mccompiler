int main(int argc, char **argv) {
  int buf[10];
  char buf2[20];
  buf[2] = 10;

  itoa(buf[2], buf2);

  puts(buf2);
  puts(argv[0]);

  return 0;
}