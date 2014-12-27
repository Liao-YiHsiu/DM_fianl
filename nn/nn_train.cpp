#include <iostream>
#include <sstream>
#include <string>
#include <fstream>
#include <vector>
#include <stdlib.h>
#include <stdio.h>
#include <ctime>
#include <math.h>
#include "nn.h"

using namespace std;

void train(vector<Edge> &edges, vector< vector<Idea> > &ideas, float ita, int N);

void Usage(char* progName){
   fprintf(stderr, "Usage: %s graph.txt training.txt iteration model.txt\n", progName);
   exit(-1);
}

int main(int argc, char** argv){
   if(argc != 5){
      Usage(argv[0]);
   }

   srand(time(NULL));
   int N;
   vector<Edge> edges;
   vector< vector<Idea> > ideas;
   int iteration = atoi(argv[3]);
   
   fprintf(stderr, "Start reading graph\n");
   // read graph and initialize 
   N = readGraph(argv[1], edges);

   fprintf(stderr, "Start reading training data\n");
   // read training data
   readTrain(argv[2], ideas);

   fprintf(stderr, "Start training.\n");
   // start training data
   for(int i = 0; i < iteration; ++i){ 
      train(edges, ideas, 1e-12, N);
      writeModel(argv[4], edges);
   }

   fprintf(stderr, "Start writing out model\n");
   writeModel(argv[4], edges);

   return 0;
}

void train(vector<Edge> &edges, vector< vector<Idea> > &ideas, float ita, int N){
   vector<float> grad( edges.size() , 0);

   vector<float> v0;
   vector<float> y;
   vector<float> v;
   vector<float> diff;
   float err = 0;
   int errN = 0;
   
   // random permutation
   vector<int> perm(ideas.size());
   for(int i = 0, size = ideas.size(); i < size; i++)
      perm[i] = i;

  // for(int i = 0, size = ideas.size(); i < size; i++){
  //    int s = rand() % size;
  //    int tmp = perm[i];
  //    perm[i] = perm[s];
  //    perm[s] = tmp;
  // }

   for(int i = 0, iSize = ideas.size(); i < iSize; ++i){

      v0.clear(); v0.resize(N+1, 0.5); v0[0] = 1; // for bias term
      y.clear(); y.resize(N+1, 0.5); y[0] = 1;    // for bias term

      int now = ideas[perm[i]][0].time;
      for(int j = 0, jSize = ideas[perm[i]].size(); j < jSize; ++j)
         y[ideas[perm[i]][j].node] = ideas[perm[i]][j].degree;

      //bool first = true;

      for(int j = 0, jSize = ideas[perm[i]].size(); j < jSize; ++j){
         //y[ideas[perm[i]][j].node] = ideas[perm[i]][j].degree;
         v0[ideas[perm[i]][j].node] = ideas[perm[i]][j].degree;

         // train per month
         if( ideas[perm[i]][j].time - now > 31 ){
            now = ideas[perm[i]][j].time;

            // calculate gradient!
            // v = sigmoid(M * v0)
            // dM = 1 / (v-y) * v * (1-v) * v0 
            //if(!first){
            v.clear(); v.resize(N+1, 0); 
            diff.clear(); diff.resize(N+1, 0);

            // do matrix-vector multiplication
            for(int k = 0, kSize = edges.size(); k < kSize; ++k){
               v[edges[k].row] += edges[k].weight * v0[edges[k].col];
            }

            for(int k = 0; k < N+1; ++k)
               v[k] = sigmoid(v[k]);

            // diff = 1/(v-y) * v * (1-v)
            for(int k = 0; k < N+1; ++k){
               diff[k] = (v[k] - y[k]) * v[k] * (1 - v[k]);
               //if(!isfinite(diff[k])) diff[k] = GRAD_MAX;
               err += (v[k] - y[k]) * (v[k] - y[k]);
               errN++;
            }

            for(int k = 0, kSize = edges.size(); k < kSize; ++k){
               grad[k] += diff[edges[k].row] * v0[edges[k].col];
            }
            //}

            //v0 = y;
            //first = false;
         }
      }

      //fprintf(stderr, ".");
      // update edges
      //if(i%10 == 9){
      //   for(int j = 0, jSize = edges.size(); j < jSize; ++j)
      //      edges[j].weight -= ita * grad[j];
      //   grad.clear();
      //   grad.resize(edges.size(), 0);
      //}
   }
   //fprintf(stderr, "\n");
   for(int i = 0, iSize = edges.size(); i < iSize; ++i)
      edges[i].weight -= ita * grad[i];
   fprintf(stdout, "%lf\n", err/errN);
}

