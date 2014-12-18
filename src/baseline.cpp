#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>
#include <sstream>
#include <string>
#include <vector>
#include <map>

using namespace std;
void Usage(char *progName){
   fprintf(stderr, "Usage: %s graph.txt query.txt answer.txt\n", progName);
   exit(-1);
}

typedef vector< vector<int> > Graph;

void readGraph(char *fileName, Graph &g){
   ifstream fin(fileName);
   string s;
   int index, tmp;
   while(getline(fin, s)){
      stringstream sin(s);
      sin >> index;
      g.resize(index+1);
      while(sin >> tmp){
         g[index].push_back(tmp);
      }
   }
}

map <int, int> mymap;
void neighbor(const Graph &g, const vector<int>& arr, vector<int>& nbr){
   mymap.clear();
   for(int i = 0, size = arr.size(); i < size; ++i){
      // neighbor includes itself.
      mymap[arr[i]] = 1;
      for (int j = 0, s = g[arr[i]].size(); j < s; ++j)
         mymap[g[arr[i]][j]] = 1;
   }
   nbr.clear();
   for(map<int,int>::iterator it = mymap.begin(); it != mymap.end(); ++it)
      nbr.push_back(it->first);
}
void swap(int &a, int &b){
   int tmp = a;
   a = b;
   b = tmp;
}

int main(int argc, char** argv){
   if( argc != 4 )
      Usage(argv[0]);

   Graph g;
   readGraph(argv[1], g);

   vector<int> C, C_;
   vector<int> score;
   vector<int> initAdp;

   ifstream fqu(argv[2]);
   ofstream fout(argv[3]);
   string s;
   while(getline(fqu, s)){
      stringstream sin(s);
      int tmp;
      initAdp.clear();
      while(sin >> tmp){
         initAdp.push_back(tmp);
      }

      C.clear();
      C_.clear();
      score.clear();
      // find neighbor of initAdp
      neighbor(g, initAdp, C);
      neighbor(g, C, C_);
      score.resize(C_.size());


      for(int i = 0, isize = C_.size(); i < isize; ++i){
         for(int j = 0, jsize = initAdp.size(); j < jsize; ++j)
            for(int k = 0, ksize = g[C_[i]].size(); k < ksize; ++k){
               if(g[C_[i]][k] == initAdp[j]) score[i]++;
            }
      }
      for(int i = 0, isize = initAdp.size(); i < isize; ++i)
         for(int j = 0, jsize = C_.size(); j < jsize; ++j)
            if(C_[j] == initAdp[i]) score[j] = 0;

      //sort
      for(int i = 0,size = C_.size(); i < size; ++i)
         for(int j = i+1; j < size; ++j)
            if(score[i] < score[j]){
               swap(score[i], score[j]);
               swap(C_[i], C_[j]);
            }
      
      for(int i = 0, size = C_.size(); i < size && i < 100; ++i)
         fout << C_[i] << " ";
      fout << endl;
   }
   return 0;
}
