#include <iostream>
#include <stdlib.h>
#include <armadillo>

using namespace std;
using namespace arma;

int main(int argc, char** argv){
   srand(time(NULL));
   mat A = randu<mat>(30000, 1001);
   mat U;
   vec s;
   mat V;
   svd(U, s, V, A);
   return 0;
}
