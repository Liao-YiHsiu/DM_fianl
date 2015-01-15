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
   fprintf(stderr, "Usage: %s graph.txt [model_out.txt]\n", progName);
   exit(-1);
}

void build(vector<array> &model, vector<int> &hidden);

int main(int argc, char* argv[]){
   ostream *fout;
   if(argc == 3){
      fout = new ofstream(argv[2]);
   }else if(argc == 2){
      fout = &cout;
   }else
      Usage(argv[0]);

   deviceset(0);
   info();

   vector< vector<Edge> > edges;

   vector< array > model;

   readGraph(argv[1], edges);

   vector<int> hidden;
   hidden.push_back(edges.size());
   hidden.push_back(200);
   hidden.push_back(edges.size());

   build(model, hidden);
   writeModel(*fout, model); 

   if(fout != &cout)
      delete fout;

   return 0;
}

void build(vector<array> &model, vector<int> &hidden){

   model.clear();
   model.resize(hidden.size() -1);
   // random initialize.
   for(int i = 0, iSize = hidden.size()-1; i < iSize; ++i){
      model[i] = 0.1*randn(hidden[i+1]+1, hidden[i]+1);
   }
}
