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

   vector<int> traveled(edges.size(), 0); 
   int color = 0;
   // search graph through init adopters.
   ans.clear();
   ans.resize(initAdpts.size());
   for(int i = 0, iSize = initAdpts.size(); i < iSize; ++i){

      vector<int> Adopters = initAdpts[i];

      color ++;
      for(int j = 0, jSize = Adopters.size(); j < jSize; ++j)
         traveled[j] = color;

      ans[i].resize(max);
      // search top each time.
      for(int j = 0; j < max; j++){
         int M = -1;
         int index = -1;
         for(int k = 0, kSize = Adopters.size(); k < kSize; ++k){
            for(int l = 0, lSize = edges[Adopters[k]].size(); l < lSize; ++l){
               int point = edges[Adopters[k]][l].from;
               if(traveled[point] == color)
                  continue;

               if(prob[point] > M){
                  M = prob[point];
                  index = point;
               }
            }
         }

         traveled[index] = color;
         ans[i][j] = index;
      }
   }
}
