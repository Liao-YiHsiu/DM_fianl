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

Real train(vector< vector<Edge> > &edges, vector< vector<Idea> > &ideas, vector< vector<Real> > &moment, Real ita, bool randPerm = true);

void Usage(char* progName){
   fprintf(stderr, "Usage: %s model_in.txt training.txt iteration learning_rate model_out.txt\n", progName);
   exit(-1);
}

int main(int argc, char** argv){
   if(argc != 6){
      Usage(argv[0]);
   }

   //srand(time(NULL));
   vector< vector<Edge> > edges;
   vector< vector<Edge> > edges_back;
   vector< vector<Idea> > ideas;
   vector< vector<Real> > moment;
   int iteration = atoi(argv[3]);
   Real ita = atof(argv[4]);
   
   fprintf(stderr, "Start reading models\n");
   // read graph and initialize 
   readModel(argv[1], edges);

   fprintf(stderr, "Start reading training data\n");
   // read training data
   readTrain(argv[2], ideas);

   // initialize moment.
   moment.resize(edges.size());
   for(int i = 0, size = edges.size(); i < size; ++i)
      moment[i].resize(edges[i].size(), 0);

   fprintf(stderr, "Start training.\n");
   // start training data
   Real perr = 1;
   for(int i = 0; i < iteration; ++i){ 
      Real err = train(edges, ideas, moment, ita);
      fprintf(stderr, "ita = %e, err = %e\n", ita, err);
      

      if(err > perr){
         ita /= 2;
         edges = edges_back;
         
         for(int i = 0, size = edges.size(); i < size; ++i){
            moment[i].clear();
            moment[i].resize(edges[i].size(), 0);
         }
         fprintf(stderr, "-------------------------------\n");
      }else{
         ita *= 1.1;
         edges_back = edges;
      }
      perr = err;

      writeModel(argv[5], edges);
   }

   fprintf(stderr, "Start writing out model\n");
   writeModel(argv[5], edges);

   return 0;
}

void trainEdge(int target, Real ans, vector< vector<Real> > &grad, vector< vector<Edge> > &edges, vector<Real> v, Real &err, int &errN){

   Real out = 0;
   Real diff;

   for(int i = 0, size = edges[target].size(); i < size; ++i)
      out += v[edges[target][i].from] * edges[target][i].weight;

   out = sigmoid(out);
   diff = out * (1 - out) * (out - ans);
   //diff = out-ans;
   
   for(int i = 0, size = edges[target].size(); i < size; ++i)
      grad[target][i] += diff * v[edges[target][i].from];

   err += (out - ans) * (out - ans);
   errN ++;

}

Real train(vector< vector<Edge> > &edges, vector< vector<Idea> > &ideas, vector< vector<Real> > &moment, Real ita, bool randPerm){
   vector< vector<Real> > grad(edges.size());
   for(int i = 0, iSize = edges.size(); i < iSize; ++i)
      grad[i].resize( edges[i].size(), 0);

   vector<Real> v0;
   vector<Real> v;
   
   vector<int> trainset;

   Real err = 0;
   int errN = 0;
   Real neutral = 0.5;
   //Real neutral = 0;
   
   // random permutation
   vector<int> perm(ideas.size());
   for(int i = 0, size = ideas.size(); i < size; i++)
      perm[i] = i;

   if(randPerm){
      for(int i = 0, size = ideas.size(); i < size; i++){
         int s = rand() % size;
         int tmp = perm[i];
         perm[i] = perm[s];
         perm[s] = tmp;
      }
   }

//v0.resize(edges.size(), neutral); v0[0] = 1;
   v0.resize(edges.size(), 0); v0[0] = 1;

   for(int i = 0, iSize = ideas.size(); i < iSize; ++i){

      //v.clear(); v.resize(edges.size(), neutral); v[0] = 1; // for bias term
      v.clear(); v.resize(edges.size(), 0); v[0] = 1; // for bias term

      int now = ideas[perm[i]][0].time;
      //bool first = true;

      trainset.clear();

      for(int j = 0, jSize = ideas[perm[i]].size(); j < jSize; ++j){

         // train start when date is not the same
         if( ideas[perm[i]][j].time != now ){
            now = ideas[perm[i]][j].time;

            for(int k = 0, kSize = trainset.size(); k < kSize; k++){
               int target = trainset[k];

               // for the neutral one
               //trainEdge(target, neutral, grad, edges, v0, err, errN);

               // for the turn on one
               trainEdge(target, v[target], grad, edges, v, err, errN);
            }

            trainset.clear();
         }

         //v[ideas[perm[i]][j].node] = ideas[perm[i]][j].degree;
         v[ideas[perm[i]][j].node] = 1;
         trainset.push_back(ideas[perm[i]][j].node);
      }

      // watch out for the last one!!!!
      for(int k = 0, kSize = trainset.size(); k < kSize; k++){
         int target = trainset[k];

         // for the neutral one
         trainEdge(target, neutral, grad, edges, v0, err, errN);

         // for the turn on one
         trainEdge(target, v[target], grad, edges, v, err, errN);
      }

      //fprintf(stderr, ".");
      // update edges
      //if(i%10 == 9){
      //   for(int j = 0, jSize = edges.size(); j < jSize; ++j)
      //      edges[j].weight -= ita * grad[j];
      //   grad.clear();
      //   grad.resize(edges.size(), 0);
      //}

      // update after each idea!!!
      for(int j = 0, jSize = edges.size(); j < jSize; ++j)
         for(int k = 0, kSize = edges[j].size(); k < kSize; ++k){
            moment[j][k] = moment[j][k] * 0.9 + grad[j][k];
            //edges[j][k].weight -= ita * grad[j][k];
            edges[j][k].weight -= ita * moment[j][k];
            grad[j][k] = 0;
         }
   }



   return err/errN;
}

