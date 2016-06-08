char *arr[10];
char aa[100];
int n;
int main(int c, char** v) { /* main tem dois parâmetros de entrada */
  int i;
  for (i = 0; i < n; i=i+1)
    arr[i] = &aa[10*i];
  return 0;
}

