#include <stdlib.h>
#include <stdio.h>
#include "nn.h"
#define BUF 102400
#define INF 100000000
using namespace std;

void Usage(char* progName){
   fprintf(stderr, "usage: %s graph.txt training.txt output_dir\n", progName);
   exit(-1);
}

int readGraph(char* fileName, vector< vector<int> > &graph); 
void parse(vector< vector<int> >& graph, vector< vector<Idea> > &ideas, int N, char* dirName);

int main(int argc, char* argv[]){
   if(argc != 4)
      Usage(argv[0]);

   int N;
   vector< vector<int> > graph;
   vector< vector<Idea> > ideas;

   N = readGraph(argv[1], graph);
   
   readTrain(argv[2], ideas);

   parse(graph, ideas, N, argv[3]);


   return 0;
}

int readGraph(char* fileName, vector< vector<int> > &graph){
   ifstream fin(fileName);
   string line;
   int id, tmp;
   int N = -1;
   while(getline(fin, line)){
      stringstream sin(line);
      sin >> id;
      if( id > N ) N = id;
      graph.resize(id+1);
      while( sin >> tmp )
         graph[id].push_back(tmp);
   }
   return N;
}

void parse(vector< vector<int> >& graph, vector< vector<Idea> > &ideas, int N, char* dirName){
   char fileName[BUF];
   int time;

   for(int n = 1; n <= N; ++n){
      fprintf(stderr, "%d\n", n);
      sprintf(fileName, "%s/%05d.txt", dirName, n);
      ofstream fout(fileName);

      vector<float> arr;
      vector<int> mapping;

      mapping.resize(N+1, -1);
      for(int i = 0, iSize = graph[n].size(); i < iSize; ++i)
         mapping[graph[n][i]] = i;

      for(int i = 0, iSize = ideas.size(); i < iSize; ++i){

         arr.clear();   
         arr.resize(graph[n].size(), 0.5);

         time = INF;
         float value;
         bool change = false;

         for(int j = 0, jSize = ideas[i].size(); j < jSize; ++j){ 
            if(ideas[i][j].time > time)
               break;
            
            if(mapping[ideas[i][j].node] != -1){
               arr[ mapping[ideas[i][j].node] ] = ideas[i][j].degree;
               change = true;
            }

            // finally got 
            if(ideas[i][j].node == n){
               time = ideas[i][j].time;
               value = ideas[i][j].degree;
            }
         }

         if(time != INF && change){
            fout << value << " ";
            for(int j = 0, jSize = arr.size(); j < jSize; ++j)
               fout << j+1 << ":" << arr[j] << " ";
            fout << endl;
         }
      }
   } 
}
