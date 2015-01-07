#include<iostream>
#include<fstream>
#include<stdlib.h>
#include<stdio.h>
#include<vector>
#include<sstream>
#include<string>
#include<algorithm>
#include "nn.h"
using namespace std;

void Usage(char* progName){
   fprintf(stderr, "Usage: %s model.txt query.txt answer.txt\n", progName);
   exit(-1);
}

void test(vector< vector<Edge> >& edges, vector< vector<int> > &initAdts, vector< vector<int> > &ans, int max);

int main(int argc, char** argv){
   if(argc != 4)
      Usage(argv[0]);

   vector< vector<Edge> > edges;
   vector< vector<int> > initAdts;
   vector< vector<int> > ans;

   int N;

   fprintf(stderr, "Start reading models.\n");
   // read trained models
   readModel(argv[1], edges);

   fprintf(stderr, "Start reading initial Adopters.\n");
   // read initial adopters
   readTest(argv[2], initAdts);

   fprintf(stderr, "Start testing.\n");
   // test
   test(edges, initAdts, ans, 100);

   fprintf(stderr, "Start writing answers.\n");
   writeAns(argv[3], ans);

   return 0;
}

void test(vector< vector<Edge> >& edges, vector< vector<int> > &initAdts, vector< vector<int> > &ans, int max){
   vector<Real> v0;
   vector<Real> v;
   vector<int> index;
   vector< pair<int, Real> > arr;
   vector<int> top;

   Real neutral = 0.5;
   
   for(int i = 0, iSize = initAdts.size(); i < iSize; ++i){
      v0.clear(); v0.resize(edges.size(), neutral); v0[0] = 1; // for bias term

      // inference 1 time.
      for (int j = 0; j < 5; ++j){

         for(int k = 0, kSize = initAdts[i].size(); k < kSize; ++k){
            v0[initAdts[i][k]] = 1;
         }

         v.clear(); v.resize(edges.size(), 0); 
         // v = M*v0;
         for(int k = 0, kSize = edges.size(); k < kSize; ++k)
            for(int l = 0, lSize = edges[k].size(); l < lSize; ++l)
               v[k] += edges[k][l].weight * v0[edges[k][l].from];

         for(int k = 0; k < edges.size(); ++k)
            v[k] = sigmoid(v[k]);

         v0 = v;
         //fprintf(stderr, ".");
      }

      //fprintf(stderr, "\n");
      index.clear(); index.resize(edges.size(), 0);
      for(int j = 0; j < edges.size(); ++j)
         index[j] = j;

      // eliminate initial adopters and 0
      for(int j = 0, jSize = initAdts[i].size(); j < jSize; ++j)
         index[initAdts[i][j]] = -1;
      index[0] = -1;

      // sort index by v
      arr.clear();
      pair<int, Real> pTmp;
      for(int j = 0; j < edges.size(); j++)
         if(index[j] != -1)
            arr.push_back(pair<int, Real>(index[j], v0[j]));
      
      sort(arr.begin(), arr.end(), mycomp);

      top.clear();
      for(int j = 0, jSize = arr.size(); j < max && j < jSize; ++j)
         top.push_back(arr[j].first);
      ans.push_back(top);
   }
}

