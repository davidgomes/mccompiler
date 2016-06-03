int fact(int n){
  if(n == 0)
    return 1;
  return n * fact(n-1);
}

int main(int ac, char **av) {
  char buffer[20];

  itoa(fact(5), buffer);

  puts(buffer);

  return 0;
}