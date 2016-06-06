int main(int argc, char **argv) {
  int i;

  for (i = 0; i < argc; i = i + 1) {
    puts(argv[i]);
  }

  return 0;
}
