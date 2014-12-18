#include <cmath>
#include <iostream>
using namespace std;
int main(){
   float f = 1/0.0 +1;
   cout << f << isnan(f) << endl;
}
