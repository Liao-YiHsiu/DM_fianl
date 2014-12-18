#include <iostream>
#include <assert.h>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>
#include <algorithm>
#include <string>
#include <cstring>
#include "nn.h"
using namespace std;

void Usage(const char* progName){
   fprintf(stderr, "usage: %s <command> [<args>]\n", progName);
   fprintf(stderr, "\n");
   fprintf(stderr, "Commands include:\n");
   fprintf(stderr, "    tr      training data format for libmf\n");
   fprintf(stderr, "    te      testing data format for libmf\n");
   fprintf(stderr, "    ev      evaluating data format from libmf\n");
   exit(-1);
}

void Usagetr(const char* progName){
   fprintf(stderr, "usage: %s tr training.txt mf.train\n", progName);
   exit(-1);
}

void Usagete(const char* progName){
   fprintf(stderr, "usage: %s te graph.txt testing_q.txt mf.test\n", progName);
   exit(-1);
}

void Usageev(const char* progName){
   fprintf(stderr, "usage: %s ev testing_q.txt mf.test.out  100 mf_a.txt\n", progName);
   exit(-1);
}

void readScores(const char* fileName, vector< vector<float> > &scores, int N);
void writeEv(const char* fileName, vector< vector<int> > initAdp, vector< vector<float> > &scores, int num);
void writeTe(const char* fileName, vector< vector<int> >& initAdp, int N);
void writeMF(const char* fileName, vector< vector<Idea> > &ideas);
int mainTr(int argc, const char* argv[]);
int mainTe(int argc, const char* argv[]);
int mainEv(int argc, const char* argv[]);

int main(int argc, const char* argv[]){
   if(argc < 2)
      Usage(argv[0]);

   if(strncmp( argv[1], "tr", 2) == 0){
      return mainTr(argc, argv);

   }else if(strncmp(argv[1], "te", 2) == 0){
      return mainTe(argc, argv);

   }else if(strncmp(argv[1], "ev", 2) == 0){
      return mainEv(argc, argv);

   }else
      Usage(argv[0]);


   return 0;
}

int mainTr(int argc, const char* argv[]){
   if (argc != 4)
      Usagetr(argv[0]);

   vector< vector<Idea> > ideas;

   readTrain(argv[2], ideas);

   writeMF(argv[3], ideas);

   return 0;
}

int mainTe(int argc, const char* argv[]){
   if (argc != 5)
      Usagete(argv[0]);

   vector<Edge> edges;
   vector< vector<int> > initAdp;

   int N = readGraph(argv[2], edges);

   readTest(argv[3], initAdp);

   writeTe(argv[4], initAdp, N);

   return 0;
}

int mainEv(int argc, const char* argv[]){
   if (argc != 6)
      Usageev(argv[0]);

   vector< vector<int> > initAdp;
   vector< vector<float> > scores;

   readTest(argv[2], initAdp);

   readScores(argv[3], scores, initAdp.size());

   // do sorting and eliminate initAdp from toplist.
   // and output to file.
   writeEv(argv[5], initAdp, scores, atoi(argv[4]));

   return 0;
}

void readScores(const char* fileName, vector< vector<float> > &scores, int N){
   ifstream fin(fileName);
   vector<float> arr;
   float tmp;
   while(fin >> tmp)
      arr.push_back(tmp);
   assert(arr.size()%N == 0);

   scores.resize(N);
   for(int i = 0; i < N; ++i)
      for(int j = 0, jSize = arr.size()/N; j < jSize; ++j)
         scores[i].push_back(arr[i*N+j]);

}

void writeEv(const char* fileName, vector< vector<int> > initAdp, vector< vector<float> >&scores, int num){
   ofstream fout(fileName);
   for(int i = 0, iSize = initAdp.size(); i < iSize; ++i){
      vector< pair<int, float> > arr;
      for(int j = 0, jSize = scores[i].size(); j < jSize; ++j)
         arr.push_back(pair<int, float>(j, scores[i][j]));

      for(int j = 0, jSize = initAdp[i].size(); j < jSize; ++j)
         arr[initAdp[i][j] - 1].first = -1;
      sort(arr.begin(), arr.end(), mycomp);
      int counter = 0;
      for(int j = 0, jSize = arr.size(); j < jSize; ++j){
         if( counter >= num) break;
         if( arr[j].first != -1){
            fout << arr[j].first+1 << " ";
            counter++;
         }
      }
      fout << endl;
   }
}

void writeTe(const char* fileName, vector< vector<int> >& initAdp, int N){
   ofstream fout(fileName);
   vector<float> arr;

   for(int i = 0, iSize = initAdp.size(); i < iSize; ++i){
      arr.clear();
      arr.resize(N, 0);

      for(int j = 0, jSize = initAdp[i].size(); j < jSize; ++j)
         arr[ initAdp[i][j] -1 ] = 1;

      for(int j = 0; j < N; ++j)
         fout << j+1 << " " << i << " " << arr[j] << endl;
   }
}

void writeMF(const char* fileName, vector< vector<Idea> > &ideas){
   ofstream fout(fileName);
   for(int i = 0, iSize = ideas.size(); i < iSize; ++i)
      for(int j = 0, jSize = ideas[i].size(); j < jSize; ++j){
         fout << ideas[i][j].node << " " <<  i+1 << " " << ideas[i][j].degree << endl; 
      }
}
