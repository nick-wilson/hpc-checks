#include <iostream>
#include <stdlib.h>
int main () {
  using namespace std;
  cout << "sizeof(void)=" << sizeof(void *) << endl <<
          "sizeof(int)=" << sizeof(int *) << endl <<
          "sizeof(long)=" << sizeof(long *) << endl <<
          "sizeof(float)=" << sizeof(float *) << endl <<
          "sizeof(double)=" << sizeof(double *) << endl;
}
