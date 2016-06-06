int f(int* a){
  return 10;
}

int main(void){
  int **a;
  int b;
  char* buffer;

  buffer = "Hello World";

  /**a = 10;
  b = *a;*/
  b = f(*a);
  itoa(b, buffer);
  puts(buffer);

  return 0;
}
