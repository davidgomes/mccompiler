int main(int ac, char **av) {
  char buffer[20];

  int c;

  /*c = 4 < 7 == 3 > 5 == 0;*/
  c = 7 >= 7;

  itoa(c, buffer);

  puts(buffer);

  return 0;
}