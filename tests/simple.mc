char buf[10];

int main(int argc, char **argv) {
  int a;
  a = 5+5+5;

  puts("A resposta e:");
  puts(itoa(1234, buf));

  if (a == 5) {
    main();
  } else {
    main();
  }

  return 0;
}
