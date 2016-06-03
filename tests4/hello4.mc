int cenas(void) {
  return 5;
}

int main(int ac, char **av) {
  char buffer[20];

  itoa(cenas(), buffer);

  puts(buffer);

  return 0;
}