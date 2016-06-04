int main(int ac, char **av) {
  char buffer[20];

  char a, b;
  int c;

  puts(itoa(c == a >= b != c, buffer));
  puts(itoa(a != b < b != c, buffer));
  puts(itoa(c > a >= c == c, buffer));
  puts(itoa(c == a >= b != c, buffer));
  puts(itoa(1 == 1 >= 1, buffer));
  puts(itoa(b > c, buffer));
  puts(itoa(!(1 == 1 >= 1), buffer));

  return 0;
}