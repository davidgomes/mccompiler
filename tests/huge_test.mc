int a[5], ***b, **b[10];


void main(void)
{
    printf("Hello World\n");
    return 0;
}

int main2(int a, int b, int *c)
{
   int number;

   printf();
   scanf("%d",&number);

   printf("Integer entered by you is %d\n", number);

   return 0;
}

int main3(void)
{
   int x;
   x = 1;

   if ( x == 1 )
      printf("x is equal to one.\n");
   else
      printf("For comparison use == as = is the assignment operator.\n");

   return 0;
}


int main4(void)
{
   int n, c;

   printf("Enter a number\n");
   scanf("%d", &n);

   if ( n == 2 )
      printf("Prime number.\n");
   else
   {
       for ( c = 2 ; c <= n - 1 ; c = c + 1 )
       {


       }
       if ( c != n )
          printf("Not prime.\n");
       else
          printf("Prime number.\n");
   }
   return 0;
}

int main5(int argc, char **argv)
{
   int c;

   printf("Number of command line arguments passed: %d\n", argc);

   for ( c = 0 ; c < argc ; c = c + 1)
      printf("%d. Command line argument passed is %s\n", c+1, argv[c]);

   return 0;
}

int main6(void)
{
    int array[100], n, c;

    printf("Enter the number of elements in array\n");
    scanf("%d", &n);

    printf("Enter %d elements\n", n);

    for ( c = 0 ; c < n ; c = c + 1 )
        scanf("%d", &array[c]);

    printf("Array elements entered by you are:\n");

    for ( c = 0 ; c < n ; c = c + 1 )
        printf("array[%d] = %d\n", c, array[c]);

    return 0;
}

void* my_function(void);

int** main7(void)
{
   printf("Main function.\n");

   my_function();

   printf("Back in function main.\n");

   return 0;
}

void *my_function(void)
{
   printf("Welcome to my function. Feel at home.\n");
}

int main8(void) {

   printf("Writing comments is very useful.\n");

   /*
    * Multi line comment syntax
    * Comments help us to understand code later easily.
    * Will you write comments while developing programs ?
    */

   printf("Good luck c programmer.\n");

   return 0;
}

void main9(void) {
	int a;
}

int** main10(int a, int ***b) {
	int b;
	return ******c;
}
