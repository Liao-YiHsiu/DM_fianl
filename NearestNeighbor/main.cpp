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
   vector<int> empty;

   vector<vector<int> > ideas_person;
   ideas_person.resize(ideas.size());

   for(int i = 0, iSize = ideas.size(); i < iSize; ++i){
      ideas_person[i].resize(edges.size(), 0);

      for(int j = 0, jSize = ideas[i].size(); j < jSize; ++j){
         prob[ideas[i][j].node] += 1;
         ideas_person[i][ideas[i][j].node] = 1;
         //prob[ideas[i][j].node] += ideas[i][j].degree;
      }
   }

   ans.clear();
   ans.resize(initAdpts.size());

   vector<Real> sim(ideas.size());
   int closestN = 1000;
   // find 100 closest point to initAdpts and do average
   for(int i = 0, iSize = initAdpts.size(); i < iSize; ++i){

      // clear similarity function.
      for(int j = 0, jSize = sim.size(); j < jSize; ++j)
         sim[j] = 0;
      
      // calculate similarity.
      for(int j = 0, jSize = ideas.size(); j < jSize; ++j){
         for(int k = 0, kSize = initAdpts[i].size(); k < kSize; ++k)
            sim[j] += ideas_person[j][initAdpts[i][k]];
      }

      rankAns(top, sim, empty, closestN);

      vector<Real> score(edges.size(), 0);
      for(int j = 0, jSize = top.size(); j < jSize; ++j){
        // if(sim[top[j]] * 4 < initAdpts[i].size()) break;
         for(int k = 0, kSize = ideas[top[j]].size(); k < kSize; ++k){
            score[ideas[top[j]][k].node] += 1;
         }
      }

      rankAns(ans[i], score, initAdpts[i], max);
   }
}
