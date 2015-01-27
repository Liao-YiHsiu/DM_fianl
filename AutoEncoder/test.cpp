#include <iostream>
#include <arrayfire.h>
#include <stdlib.h>
#include <stdio.h>
#include <algorithm>
#include "nn.h"
#include "libnn.h"
using namespace std;
using namespace af;

void Usage(char* progName){
   fprintf(stderr, "Usage: %s model.txt q1.txt a1.txt\n", progName);
   exit(-1);
}

void test(vector<array> &model, vector< vector<int> > &initAdpts, vector< vector<int> > &ans);

int main(int argc, char* argv[]){
   if(argc != 4)
      Usage(argv[0]);

   deviceset(0);
   info();

   vector<array> model;
   vector< vector<int> > initAdpts;
   vector< vector<int> > ans;

   ifstream fin(argv[1]);
   readModel( fin, model);
   readTest( argv[2], initAdpts);

   test(model, initAdpts, ans);

   writeAns(argv[3], ans);

   return 0;
}

void test(vector<array> &model, vector< vector<int> > &initAdpts, vector< vector<int> > &ans){

   vector<array> layers;
   layers.resize(model.size()+1);
   int N = model[0].dims(1);

   vector< vector<Real> > input(initAdpts.size());

   for(int i = 0, iSize = initAdpts.size(); i < iSize; ++i){
      input[i].resize(N, 0);
      for(int j = 0, jSize = initAdpts[i].size(); j < jSize; ++j)
         input[i][initAdpts[i][j]] = 1;
   }
   layers[0] = vec2arr(input);

   cout << layers[0].dims() << endl;
   cout << model[0].dims() << endl;
   cout << model[1].dims() << endl;

   forward(model, layers, sigmoid);

   vector< vector<Real> > rank;
   rank = arr2vec(layers[layers.size()-1]);


   ans.clear();
   ans.resize(initAdpts.size());
   for(int i = 0, iSize = initAdpts.size(); i < iSize; ++i){
      initAdpts[i].push_back(0);
      rankAns(ans[i], rank[i], initAdpts[i], 100);
   }

}
