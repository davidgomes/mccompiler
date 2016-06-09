char buf[15];

char aa[100], a;
void *arr[20];

char 

int main(void) {
  char *p1, *p2, *p3;
  void **v1, **v2;
  int n;

  aa[0] = 'a';
  aa[1] = 'b';

  aa[0] = 5;

  p1 = aa;
  p2 = aa + 1;

  v1 = arr;
  v2 = arr + 1;

  puts(itoa(v2 - v1, buf));
}

