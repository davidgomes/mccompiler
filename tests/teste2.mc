void *p;

void p(void);

void p(void) {} /* devia dar conflicting types */


int A;
int *A(int); /* nao devia ser no asterisco mas sim no nome */

void k;