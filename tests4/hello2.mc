int chico(void) {
  puts("hey");
  return 0;
}

int main(int argc, char **argv) {
  int k;
  char buffer[20];

  puts(argv[1]);

  k = atoi(argv[2]);
  k = k + 1;


  chico();
  /*itoa(k, buffer);
  puts(buffer);*/

  return 0;
}