char *itoa(int n, char* buf) {
  sprintf(buf, "%d", n);
  return buf;
}

int fibonacci_recursive(int n)
{
  if (n == 0)
  {
      return 0;
   }
   if (n == 1) {
         return 1;
   }
   return fibonacci_recursive(n - 1) + fibonacci_recursive(n - 2);
}

char buf[15];

int main(void)
{
   int n, first, second, next, c;
   n = 0;
   second = 1;
   next = 0;
   c = 0;

   puts("Enter the number of terms");
   n = 20;

   puts("First %d terms of Fibonacci series are :-");

   for ( c = 0 ; c < n ; c = c + 1)
   {
      puts(itoa(fibonacci_recursive(c), buf));
   }

   return 0;
}
