int cenas(void) {
  return 5 == 5 * 4 + 3;
}

int main(int ac, char **av) {
  char buffer[20];

  itoa(cenas(), buffer);

  puts(buffer);

  return 0;
}