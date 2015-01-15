#ifndef _LIBNN_
#define _LIBNN_
#include <iostream>
#include <arrayfire.h>
#include <stdlib.h>
#include <stdio.h>
#include <algorithm>
#include "nn.h"
using namespace std;
using namespace af;

typedef array (*func)(array&);
//typedef struct{
//   array label;
//   vector< array > weight;
//   vector< array > layers;
//   vector< array > delta;
//   vector< array > drt;
//} DNN;

void writeArray(ostream &out, array &arr);
void readArray(istream &in, array &arr);

void readModel(istream &in, vector<array> &model);
void writeModel(ostream &out, vector<array> &model);

array sigmoid_(array &in);
array sigmoid(array &in);
array relu(array &in);
array relu_(array &y);

array vec2arr(vector< vector<Real> > &arr, bool transpose = false);
vector< vector<Real> > arr2vec(array &arr, bool transpose = false);

void forward(vector< array > &model, vector< array > &layers, func f);
void backward(array &label, vector< array > &model, vector< array > &layers, vector< array > &delta,
      func f_, vector< array > &ppd, float &err);


void forward(vector< array > &model, vector< array > &layers, func f){

   layers[0].row(0) = 1;

   for(int i = 0, iSize = model.size(); i < iSize; ++i){
      layers[i+1] = matmul(model[i], layers[i]);
      layers[i+1] = f(layers[i+1]);
      layers[i+1].row(0) = 1;
   }
}

void backward(array &label, vector< array > &model, vector< array > &layers, vector< array > &delta,
      func f_, vector< array > &ppd, Real &err){
  
   delta[delta.size()-1] = layers[layers.size()-1] - label;
   //err = af::norm<Real>(delta[delta.size()-1]);
   err = sum<float>(delta[delta.size()-1] * delta[delta.size()-1]);

   for(int i = model.size()-1; i >= 0; --i){
      ppd[i] = matmul( delta[i+1] * f_(layers[i+1]), layers[i] , AF_NO_TRANSPOSE, AF_TRANSPOSE);

      if( i != 0 )
         delta[i] = matmul(model[i], delta[i+1], AF_TRANSPOSE);
   }
}

array relu(array &in){
   return (in > 0)*in;
}

array relu_(array &y){
   return (y > 1e-10);
}

array sigmoid_(array &y){
   return (1 - y) * y;
}

array sigmoid(array &in){
   return 1/(1+exp(-in));
}

array vec2arr(vector< vector<Real> > &arr, bool transpose){
   Real *tmp = new Real[arr.size() * arr[0].size()];
   for(int i = 0, iSize = arr.size(); i < iSize; ++i)
      for(int j = 0, jSize = arr[i].size() ; j < jSize; ++j)
         tmp[i*jSize + j] = arr[i][j];

   array tmparr(arr[0].size(), arr.size(), tmp);
   delete[] tmp;

   if(transpose)
      return tmparr.T();
   else
      return tmparr;
}

vector< vector<Real> > arr2vec(array &arr, bool transpose){
   float *mem = new float[arr.elements()];
   arr.host(mem);
  
   vector< vector<Real> > arrtmp(arr.dims(1));
   for(int i = 0, iSize = arrtmp.size(); i < iSize; ++i)
      arrtmp[i].resize(arr.dims(0));
   
   for(int i = 0, iSize = arrtmp.size(); i < iSize; ++i)
      for(int j = 0, jSize = arrtmp[i].size(); j < jSize; ++j)
         arrtmp[i][j] = mem[i*jSize +j];

   delete[] mem;
   return arrtmp;

}

void readModel(istream &in, vector<array> &model){
   int size;
   in >> size;

   model.clear();
   model.resize(size);

   for(int i = 0; i < size; ++i)
      readArray(in, model[i]);
}

void writeModel(ostream &out, vector<array> &model){
   out << model.size() << endl;
   for(int i = 0, iSize = model.size(); i < iSize; ++i)
      writeArray(out, model[i]);
}

void writeArray(ostream &out, array &arr){
   float *mem = new float[arr.elements()];
   arr.host(mem);

   out << arr.dims() << endl;
   for(int i = 0, size = arr.elements(); i < size; ++i)
      out << mem[i] << " ";
   out << endl;
   delete[] mem;
}

void readArray(istream &in, array &arr){
   dim4 d;
   in >> d;

   float *mem = new float[d.elements()];
   for(int i = 0, size = d.elements(); i < size; ++i)
      in >> mem[i];
   arr = array(d, mem);
   delete[] mem;
}

#endif
