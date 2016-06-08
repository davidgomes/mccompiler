char *a[10];
char aa[100];
char buf[15];
int n;

void printmatrix(void) {
   int i, j;
   puts("\n");

   return;

   for (i = 0; i < n; i = i + 1) {
      for (j = 0; j < n; j = j + 1) {
         puts(itoa(*a[i * 10 + j], buf));
         }
      puts("\n\n");
   }
}

int getmarkedcol(int row) {
   int i;
   for (i = 0; i < n; i = i + 1)
      if (a[row][i] == 'Q') {
         return (i);
      }
}

int abs(int x) {
  if (x > 0) {
    return x;
  } else {
    return -x;
  }
}

int feasible(int row, int col) {
   int i, tcol;
   for (i = 0; i < n; i = i + 1) {
      tcol = getmarkedcol(i);
      if (col == tcol || abs(row - i) == abs(col - tcol))
         return 0;
   }
   return 1;
}

void nqueen(int row) {
   int i, j;
   if (row < n) {
      for (i = 0; i < n; i = i + 1) {
         if (feasible(row, i)) {
            a[row][i] = 'Q';
            nqueen(row + 1);
            a[row][i] = '.';
         }
      }
   } else {
      puts("\nThe solution is:- ");
      printmatrix();
   }
}

int main(void) {
   int i, j;

   puts("\nEnter the no. of queens:- ");
   n = 10;
   puts("Hey");

   for (i = 0; i < n; i = i + 1) {
     a[i] = aa + i * 10;
   }

   *a[1] = 'h';

   puts("hey1");

   for (i = 0; i < n; i = i + 1) {
      for (j = 0; j < n; j = j + 1) {
        /*puts(itoa(i, buf));*/
         *(a[i] + j) = '.';
     }
   }

   nqueen(0);
   return (0);
}