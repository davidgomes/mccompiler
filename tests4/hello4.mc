char arr[10];
int arr2[20];

int main(int ac, char **av) {
  arr[0] = 1;
  arr[0] = 'a';

  arr2[1] = 4;
  arr2[1] = 'b';

  puts(arr);
  puts(arr2);

  return 0;
}