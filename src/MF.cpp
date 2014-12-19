#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <fstream>
#include <vector>
#include <algorithm>
#include <assert.h>
#include "nn.h"
#define BUF 1024
using namespace std;

void Usage(const char* progName){
   fprintf(stderr, "usage: %s mf_prog graph.txt training.txt testing_q.txt answer.txt [tmp_dir]\n", progName);
   exit(-1);
}
void writeTrain(const char* fileName, vector< vector<Idea> >& ideas, vector< vector<int> >& initAdpt, int N); 
void writeTest(const char* fileName, vector< vector<Idea> >& ideas, vector< vector<int> >& initAdpt, int N);
void writeAns(const char* fileName, const char* output, vector< vector<int> >& initAdpt, int num, int N);

int main(int argc, const char* argv[]){
   if( argc < 6 || argc > 8 )
      Usage(argv[0]);

   vector<Edge> edges;
   vector< vector<Idea> > ideas;
   vector< vector<int> > initAdpt;

   char file1[BUF], file2[BUF], cmd[BUF];
   const char* mf_prog = argv[1];
   const char* tmp_dir = argc == 7? argv[6]:"./tmp";
   createDir(tmp_dir);
   int N = readGraph( argv[2], edges);
   readTrain(argv[3], ideas);
   readTest(argv[4], initAdpt);

   sprintf(file1, "%s/data.tr", tmp_dir);
   writeTrain(file1, ideas, initAdpt, N);

   sprintf(file2, "%s/data.te", tmp_dir);
   writeTest(file2, ideas, initAdpt, N);

   // convert
   sprintf(cmd, "%s convert %s %s.bin", mf_prog, file1, file1); 
   system(cmd);
   sprintf(cmd, "%s convert %s %s.bin", mf_prog, file2, file2); 
   system(cmd);

   // train
   sprintf(cmd, "%s train --tr-rmse --obj -k 50 -t 1000 -s 8 -p 0.05 -q 0.05 -g 0.003 -ub -1 -ib -1 --no-use-avg --rand-shuffle -v %s.bin %s.bin %s/model", mf_prog, file1, file1, tmp_dir);
   system(cmd);

   // predict
   sprintf(cmd, "%s predict %s.bin %s/model %s/output", mf_prog, file2, tmp_dir, tmp_dir);
   system(cmd);

   sprintf(file1, "%s/output", tmp_dir);
   writeAns(argv[5], file1, initAdpt, 100, N);


   return 0;
}

void writeTrain(const char* fileName, vector< vector<Idea> >& ideas, vector< vector<int> >& initAdpt, int N){
   ofstream fout(fileName);
   vector<float> arr;
   
   for(int i = 0, iSize = ideas.size(); i < iSize; ++i){
      arr.clear();
      arr.resize(N, 0.5);

      for(int j = 0, jSize = ideas[i].size(); j < jSize; ++j)
         arr[ ideas[i][j].node -1 ] = ideas[i][j].degree;
         //fout << ideas[i][j].node -1 << " " << i << " " << ideas[i][j].degree << endl; 

      for(int j = 0; j < N; ++j)
         fout << j << " " << i << " " << arr[j] << endl; 
   }
   int count = ideas.size();

   for(int i = 0, iSize = initAdpt.size(); i < iSize; ++i){
      arr.clear();
      arr.resize(N, 0.5);
      for(int j = 0, jSize = initAdpt[i].size(); j < jSize; ++j)
         arr[ initAdpt[i][j] -1] = 1;
      
      //for(int j = 0; j < N; ++j)
      //   fout << j << " " << i + count << " " << arr[j] << endl;

      for(int j = 0, jSize = initAdpt[i].size(); j < jSize; ++j)
         fout << initAdpt[i][j] -1 << " " << i+count << " " << 0.8 << endl;
   }

}

void writeTest(const char* fileName, vector< vector<Idea> >& ideas, vector< vector<int> >& initAdpt, int N){
   ofstream fout(fileName);
   int count = ideas.size();

   for(int i = 0, iSize = initAdpt.size(); i < iSize; ++i){
      for(int j = 0; j < N; ++j)
         fout << j << " " << count + i << " " << 0 << endl; 
   }
}

void readScores(const char* fileName, vector< vector<float> > &scores, int N){
   ifstream fin(fileName);
   vector<float> arr;
   float tmp;
   while(fin >> tmp)
      arr.push_back(tmp);
   assert(arr.size()%N == 0);

   scores.resize(arr.size()/N);
   for(int i = 0, iSize = arr.size()/N; i < iSize; ++i)
      for(int j = 0; j < N; ++j)
         scores[i].push_back(arr[i*N+j]);

}

void writeAns(const char* fileName, const char* output, vector< vector<int> >& initAdpt, int num, int N){
   ofstream fout(fileName);

   vector< vector<float> > scores;
   readScores(output, scores, N);

   for(int i = 0, iSize = initAdpt.size(); i < iSize; ++i){
      vector< pair<int, float> > arr;
      for(int j = 0, jSize = scores[i].size(); j < jSize; ++j)
         arr.push_back(pair<int, float>(j, scores[i][j]));

      for(int j = 0, jSize = initAdpt[i].size(); j < jSize; ++j)
         arr[initAdpt[i][j] - 1].first = -1;
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
