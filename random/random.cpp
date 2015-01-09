#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <algorithm>
#include "nn.h"
using namespace std;

void Usage(char* progName){
   fprintf(stderr, "Usage: %s graph.txt training.txt initAdpt.txt answer.txt\n", progName);
   exit(-1);
}

void analyze(vector< vector<int> > &ans, vector< vector<Edge> > &edges, vector< vector<Idea> > ideas, vector< vector<int> > initAdpts, int max);

int main(int argc, char* argv[]){
   if(argc != 5)
      Usage(argv[0]);

   vector< vector<Edge> > edges;
   vector< vector<Idea> > ideas;
   vector< vector<int> > initAdpts;
   vector< vector<int> > ans;

   readGraph(argv[1], edges);
   readTrain(argv[2], ideas);
   readTest(argv[3], initAdpts);

   analyze(ans, edges, ideas, initAdpts, 100);
   
   writeAns(argv[4], ans);

   return 0;
}

void analyze(vector< vector<int> > &ans, vector< vector<Edge> > &edges, vector< vector<Idea> > ideas, vector< vector<int> > initAdpts, int max){
   vector<Real> prob(edges.size(), 0);

   vector<int> index;
   vector< pair<int, Real> > arr;
   vector<int> top;

   for(int i = 0, iSize = ideas.size(); i < iSize; ++i)
      for(int j = 0, jSize = ideas[i].size(); j < jSize; ++j)
         prob[ideas[i][j].node] += 1;
         //prob[ideas[i][j].node] += ideas[i][j].degree;

   ans.clear();
   for(int i = 0, iSize = initAdpts.size(); i < iSize; ++i){

      index.clear(); index.resize(edges.size(), 0);
      for(int j = 0; j < edges.size(); ++j)
         index[j] = j;

      // eliminate initial adopters and 0
      for(int j = 0, jSize = initAdpts[i].size(); j < jSize; ++j)
         index[initAdpts[i][j]] = -1;
      index[0] = -1;

      arr.clear();
      pair<int, Real> pTmp;
      for(int j = 0; j < edges.size(); j++)
         if(index[j] != -1)
            arr.push_back(pair<int, Real>(index[j], prob[j]));
      
      sort(arr.begin(), arr.end(), mycomp);

      top.clear();
      for(int j = 0, jSize = arr.size(); j < max && j < jSize; ++j)
         top.push_back(arr[j].first);
      ans.push_back(top);
   }



}
