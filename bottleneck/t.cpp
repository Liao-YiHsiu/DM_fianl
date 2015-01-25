#include<arrayfire.h>
#include<iostream>
#include<vector>
using namespace af;
using namespace std;
int main(){
   vector<array> arr;
   vector<array> arr2;
   arr.resize(10);
   arr2.resize(10);
   for(int i = 0; i < 10; i++){
      arr[i] = constant(1, 10000, 10000);
      arr2[i] = constant(2, 10000, 10000);
   }
   for(int i = 0; i < 1000000; ++i){

      arr2[i%10] = arr[i%10] + arr2[i%10]*0.8;
      arr[i%10] += arr2[i%10];
      cout << i << endl;
   }
}
