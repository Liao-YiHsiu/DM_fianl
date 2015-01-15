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
   fprintf(stderr, "Usage: %s model_out.txt training.txt iteration learning_rate [model_in.txt]\n", progName);
   exit(-1);
}

void train(vector< vector<Idea> > &ideas, vector< array > &model, char* modelFile, float ita, int itr);

void build(vector<array> &model, vector<array> &layers, array& label, vector< vector<Real> >& in, vector< vector<Real> >& out, vector<int> hidden);


int main(int argc, char* argv[]){

   istream *fin;
   if(argc == 6){
      fin = new ifstream(argv[5]);
   }else if(argc == 5){
      fin = &cin;
   }else
      Usage(argv[0]);

   deviceset(0);
   info();

   vector< vector<Idea> > ideas;
   vector< array > model;

   readTrain(argv[2], ideas);
   readModel(*fin, model);
   if(fin != &cin)
      delete fin;

   train(ideas, model, argv[1], atof(argv[4]), atoi(argv[3]));

   return 0;
}

void train(vector< vector<Idea> > &ideas, vector< array > &model, char* outFile, float ita, int itr){

   vector< array > layers(model.size()+1);
   vector< array > delta(model.size()+1);
   vector< array > ppd(model.size());
   vector< array > momenten(model.size());
   float err;
   array label;

   int N = model[0].dims(1);


   vector< vector<Real> > input(ideas.size());
   for(int i = 0, iSize = ideas.size(); i < iSize; ++i){
      input[i].resize(N, 0);
   }

   for(int i = 0, iSize = ideas.size(); i < iSize; ++i)
      for(int j = 0, jSize = ideas[i].size(); j < jSize; ++j){
         input[i][ideas[i][j].node] = 1;
      }

   // fill in input array
   layers[0] = vec2arr( input );
   layers[0].row(0) = 1;

   // fill in output array
   label = layers[0];

   // fill in momenten array
   for(int i = 0, iSize = model.size(); i < iSize; ++i)
      momenten[i] = constant(0, model[i].dims());
   float pre_err = 1e20;

   for(int i = 0; i < itr; ++i){
      forward(model, layers, sigmoid);
      backward(label, model, layers, delta, sigmoid_, ppd, err);

      for(int j = 0, jSize = model.size(); j < jSize; ++j){
         //momenten[j] *= 0.9;
         //momenten[j] += ppd[j];
         //model[j] -= ita * momenten[j];
         model[j] -= ita * ppd[j];
      }

      cerr << "ita = " << ita <<  " error = " << err << endl;

      if( pre_err < err ){
         cout << "----------------------------" << endl;
         //for(int j = 0, jSize = momenten.size(); j < jSize; ++j)
         //   momenten[j] = 0;
         ita *= 0.7;
      }else{
         ita *= 1.05;
      }
      pre_err = err;

   }
   ofstream fout(outFile);
   writeModel(fout, model);
}

