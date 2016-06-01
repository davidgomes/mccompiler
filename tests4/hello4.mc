int a;

int f(void) {
  if (a == 5) {
    if (a == 4) {
    } else {
      return 23;
    }

    return 20;
  } else {
    return 4;

    puts("Hey");
  }

  return 2;
}

int main(int ac, char **av) {
  char buffer[20];

  a = 5;

  itoa(f(), buffer);
  puts(buffer);

  return 0;
}