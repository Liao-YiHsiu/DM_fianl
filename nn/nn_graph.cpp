#include "nn.h"

using namespace std;

void Usage(char* progName){
   fprintf(stderr, "Usage: %s graph.txt out_model.txt\n", progName);
   exit(-1);
}

int main(int argc, char* argv[]){
   if(argc != 3)
      Usage(argv[0]);

   vector<vector <Edge> > edges;

   readGraph(argv[1], edges);

   // random initialize weights.
   for(int i = 0, iSize = edges.size(); i < iSize; ++i){
      for(int j = 0, jSize = edges[i].size(); j < jSize; ++j)
         edges[i][j].weight = norm(0, 0.1); 
   }
   writeModel(argv[2], edges);
   return 0;
}
