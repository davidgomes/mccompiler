char *itoa(int n, char* buf) {
  sprintf(buf, "%d", n);
  return buf;
}

char buf[15];

void **aa[20];
void *v1, *v2;

int main(void) {
  v1 = aa;
  v2 = aa + 1;

  puts(itoa(v2 - v1, buf));
}