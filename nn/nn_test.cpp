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

void test(vector<Edge>& edges, vector< vector<int> > &initAdts, vector< vector<int> > &ans, int max, int N);

int main(int argc, char** argv){
   if(argc != 4)
      Usage(argv[0]);

   vector< vector<Edge> > edges;
   vector< vector<int> > initAdts;
   vector< vector<int> > ans;

   int N;

   fprintf(stderr, "Start reading models.\n");
   // read trained models
   N = readModel(argv[1], edges);

   fprintf(stderr, "Start reading initial Adopters.\n");
   // read initial adopters
   readTest(argv[2], initAdts);

   fprintf(stderr, "Start testing.\n");
   // test
   test(edges, initAdts, ans, 100, N);

   fprintf(stderr, "Start writing answers.\n");
   writeAns(argv[3], ans);
   return 0;
}

void test(vector<Edge>& edges, vector< vector<int> > &initAdts, vector< vector<int> > &ans, int max, int N){
   vector<float> v0;
   vector<float> v;
   vector<int> index;
   vector< pair<int, float> > arr;
   vector<int> top;
   
   for(int i = 0, iSize = initAdts.size(); i < iSize; ++i){
      v0.clear(); v0.resize(N+1, 0.5); v0[0] = 1;

      // inference 1 times.
      for (int j = 0; j < 1; ++j){
         for(int k = 0, kSize = initAdts[i].size(); k < kSize; ++k){
            v0[initAdts[i][k]] = 1;
         }

         v.clear(); v.resize(N+1, 0); 
         // v = M*v0;
         for(int k = 0, kSize = edges.size(); k < kSize; ++k)
            v[edges[k].row] += edges[k].weight * v0[edges[k].col];
         for(int k = 0; k < N+1; ++k)
            v[k] = sigmoid(v[k]);

         v0 = v;
         //fprintf(stderr, ".");
      }

      //fprintf(stderr, "\n");
      index.clear(); index.resize(N+1, 0);
      for(int j = 0; j <= N; ++j)
         index[j] = j;

      // eliminate initial adopters and 0
      for(int j = 0, jSize = initAdts[i].size(); j < jSize; ++j)
         index[initAdts[i][j]] = -1;
      index[0] = -1;

      // sort index by v
      arr.clear();
      pair<int, float> pTmp;
      for(int j = 0; j <= N; j++)
         if(index[j] != -1)
            arr.push_back(pair<int, float>(index[j], v0[j]));
      
      sort(arr.begin(), arr.end(), mycomp);

      top.clear();
      for(int j = 0, jSize = arr.size(); j < max && j < jSize; ++j)
         top.push_back(arr[j].first);
      ans.push_back(top);
   }
}

