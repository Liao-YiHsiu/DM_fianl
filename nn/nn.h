#ifndef _NN_H_
#define _NN_H_

#include <cmath>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

using namespace std;
// use coo to store data

typedef float Real;

typedef struct{
   int from;
   Real weight;
} Edge;

// degree normalize to -1 to 1
typedef struct{
   int node;
   int time;
   Real degree;
} Idea;

inline Real sigmoid(Real a){
   return 1/(1+exp(-a));
}

double norm(double mean, double std){
   double u = rand()/(double)RAND_MAX;
   double v = rand()/(double)RAND_MAX;
   return sqrt(-2*log(u)) * cos(2*M_PI*v)*std + mean;
}

void mprint(vector<Real>& arr){
   for(int i = 0, iSize = arr.size(); i < iSize; ++i)
      cout << arr[i] << " ";
}

bool mycomp(const pair<int, Real> &a, const pair<int, Real> &b){
   return a.second > b.second;
}

int date2Int(string& date){
   int y, m, d;
   sscanf(date.c_str(), "%d/%d/%d", &y, &m, &d);
   struct tm t1 = {0, 0, 0, d, m-1, y-1900};
   struct tm t2 = {0, 0, 0, 1, 0, 100}; 
   time_t time1 = mktime(&t1); 
   time_t time2 = mktime(&t2); 
   return difftime(time1, time2) / (60*60*24) + 0.5;
}

void readTrain(const char* fileName, vector< vector<Idea> > &ideas){
   ifstream fin(fileName);
   string line;
   int nowID = -1;
   int node, id;
   Real degree;
   string date;
   Idea idea;

   while(getline(fin, line)){
      stringstream sin(line);
      sin >> node >> id >> date >> degree;
      idea.node = node;
      idea.degree = degree;
      idea.time = date2Int(date);
      if(nowID != id){
         vector<Idea> newIdea;
         ideas.push_back(newIdea);
         nowID = id;
      }
      ideas.back().push_back(idea);
   }
}

void readGraph(const char* fileName, vector< vector<Edge> > &edges){
   ifstream fin(fileName);
   string line;
   int node, neighbor;

   Edge tmp_e;
   tmp_e.weight = 0;
   
   while(getline(fin, line)){

      stringstream sin(line);
      sin >> node;

      vector<Edge> tmp_arr;
      neighbor = 0; // for bias term.

      do{
         tmp_e.from = neighbor;
         tmp_arr.push_back(tmp_e);
      }while(sin >> neighbor);

      edges.resize(node+1);
      edges[node] = tmp_arr;
   }
}

void writeModel(const char* fileName, vector< vector<Edge> > &edges){
   ofstream fout(fileName);
   for(int i = 0, iSize = edges.size(); i < iSize; ++i){
      fout << i << "\t";
      for(int j = 0, jSize = edges[i].size(); j < jSize; ++j)
         fout << edges[i][j].from << " " << edges[i][j].weight << " ";
      fout << endl;
   }
}

void readModel(const char* fileName, vector< vector<Edge> > &edges){
   ifstream fin(fileName);
   string line;
   edges.clear();

   Edge e_tmp;
   int node;

   while(getline(fin, line)){
      stringstream sin(line);
      sin >> node;

      vector<Edge> tmp_arr;
      while(sin >> e_tmp.from >> e_tmp.weight){
         tmp_arr.push_back(e_tmp);
      }

      edges.resize(node+1);
      edges[node] = tmp_arr;
   }
}

void readTest(const char* fileName, vector< vector<int> > &initAdts){
   ifstream fin(fileName);
   string line;
   initAdts.clear();
   vector<int> tmpArr;
   int tmp;

   while(getline(fin, line)){
      stringstream sin(line);
      tmpArr.clear();

      while(sin >> tmp)
         tmpArr.push_back(tmp);
      initAdts.push_back(tmpArr);
   }
}

void writeAns(const char* fileName, vector< vector<int> > &ans){
   ofstream fout(fileName);
   for(int i = 0, iSize = ans.size(); i < iSize; ++i){
      for(int j = 0, jSize = ans[i].size(); j < jSize; ++j)
         fout << ans[i][j] << " ";
      fout << endl;
   }
}

void createDir(const char* dirName){
   struct stat buf;
   if( stat(dirName, &buf) != 0 ){
      if( S_ISDIR(buf.st_mode) )
         return;
      unlink(dirName);
   }
   mkdir(dirName, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
   
}

#endif
