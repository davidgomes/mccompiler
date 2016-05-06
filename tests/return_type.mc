int call(void);

int *c(void) {
  return 5;
}

void f(void) {

}

void return_void(void) {
  return f();
}

void should_be_void(void) {
  return 5;
}

int call(void) {
 return f();
}

int return_char(void) {
  char *p;
  return *p;
}