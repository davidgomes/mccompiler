int call(void);

int *c(void) {
  return 5;
}

void f(void) {

}

void return_void(void) {
  return f();
}

int call(void) {
 return f();
}

int return_char(void) {
  char *p;
  return *p;
}