#include <algorithm>
#include <iostream>
#include <fstream>
#include <utility>
#include <string>
#include <vector>
#include <math.h>
#include <map>
#include "nn.h"
#include "util.h"


using namespace std;

void find_adoptersA( int, vector< vector<float> >&, vector<int>& );
void find_adoptersB( vector<int>, vector< vector<float> >&, vector<int>& );
void find_adoptersC( vector<int>, vector< vector<float> >&, vector<int>& );

int main( int argc, char *argv[] ){

	if(argc!=5){
		cout<<"\n\033[1;31m Usage: ./naive_edge [Training] [Graph] [Testing] [Output Result]\033[0m\n\n";
		return 0;
	}
	ifstream G(argv[2]);
	vector< vector<Idea> > tr_idea;
	map< int, vector<int> > graph;
	map< int, vector<int> >::iterator it;
	vector< vector<float> > naive_edge;
	vector< vector<int> > test_adopt;
	vector< vector<int> > ans;


	cerr<<"Read training data: ";
	readTrain( argv[1], tr_idea );
	cerr<<"Done\n";
	cerr<<"Read graph: ";
	readGraph1( G, graph );
	cerr<<"Done\n";
	cerr<<"Read testing data: ";
	readTest( argv[3], test_adopt );
	cerr<<"Done\n";
	

	cerr<<"\033[31m                       0%   10   20   30   40   50   60   70   80   90   100%\033[0m\n";
	cerr<<"\033[31m                       |----|----|----|----|----|----|----|----|----|----|\033[0m\n";
	cerr<<"Naive Graph initialize: ";
	//Initialize naive graph
	naive_edge.resize( graph.size() );
	for( size_t i=0; i<graph.size(); ++i )
		naive_edge[i].resize( graph.size() );

	//Add Graph information into naive graph
	for( it=graph.begin(); it!=graph.end(); it++ )
		for( size_t i=0; i<it->second.size(); ++i )
			naive_edge[it->first-1][ it->second[i]-1 ]+=1;
	
	//Add adoptor information into naive graph
	for( size_t i=0; i<tr_idea.size(); ++i ){
		for( size_t j=1; j<tr_idea[i].size(); ++j )
			for( size_t k=0; k<=j; ++k )
				naive_edge[ tr_idea[i][k].node-1 ][ tr_idea[i][j].node-1 ] += 1;
		if( i%20==0 ) cerr<<"\033[31m\b=D\033[0m";
	}

	cerr<<"   Done\n\n";
	cerr<<"Predict testing results: ";
	//test
	ans.resize( test_adopt.size() );
	for( size_t i=0; i<test_adopt.size(); ++i ){
/*
		int j=0;
		while( ans[i].size()<100 ){
			if( j==test_adopt[i].size() )
				j=0;
			else j++;
			find_adoptersA( test_adopt[i][j], naive_edge, ans[i] );
		}
*/
		find_adoptersB( test_adopt[i], naive_edge, ans[i] );
	}
	writeAns( argv[4], ans );
	cerr<<"Done\n\n";

	return 0;
}


bool comparator ( const pair<int,float>& l, const pair<int,float>& r){
	return l.second > r.second;
}


//Find an adopter for each initial adopters
void find_adoptersA( int init, vector< vector<float> >& edge, vector<int>& ans ){
	vector< pair<int,float> >list;//pair<edge_weight,index>
	vector<int>::iterator it;
	for( size_t i=0; i<edge[init-1].size(); ++i )
		if( edge[init-1][i]!=0 ){
			pair<int,int> candidate(i+1,edge[init-1][i]);
			list.push_back(candidate);
		}
	if( list.size()==0 ) return;
	sort( list.begin(), list.end(), comparator );

	if( ans.size()==0 ){
		ans.push_back( list[0].first );
		return;
	}
		
	size_t idx(0);
	it = find( ans.begin(), ans.end(), list[idx].first );
	while( it!=ans.end() ){
		idx++;
		it = find( ans.begin(), ans.end(), list[idx].first );
	}
	ans.push_back( list[idx].first );
		
}

//Sort all the neighbors of initial adopters, and then sellect the top 100.
void find_adoptersB( vector<int> init, vector< vector<float> >& edge, vector<int>& ans ){
	vector< pair<int,float> > candidate;
	vector<int>::iterator it;
	for( size_t i=0; i<edge.size(); ++i ){
		pair<int,int> tmp(i+1,0);
		candidate.push_back(tmp);
	}

	for( size_t i=0; i<init.size(); ++i )
		for( size_t j=0; j<edge[ init[i]-1 ].size(); ++j )
			candidate[j].second += edge[ init[i]-1 ][j];
	sort( candidate.begin(), candidate.end(), comparator );

	size_t i=0;
	while( ans.size()<100 ){
		it = find( init.begin(), init.end(), candidate[i].first );
		if( it==init.end() )
			ans.push_back( candidate[i].first );
		i++;
	}

}

//For each iteration:
//	Sort all the neighbors of initial adopters, and then sellect the top one.
//	Add the neighbors of the chosen one into list, and sort again.
void find_adoptersC( vector<int> init, vector< vector<float> >& edge, vector<int>& ans ){
	vector< pair<int,float> > candidate;
	vector< pair<int,float> > cand_sort;
	vector<int>::iterator it1, it2;
	for( size_t i=0; i<edge.size(); ++i ){
		pair<int,int> tmp(i+1,0);
		candidate.push_back(tmp);
	}

	for( size_t i=0; i<init.size(); ++i )
		for( size_t j=0; j<edge[ init[i]-1 ].size(); ++j )
			candidate[j].second += edge[ init[i]-1 ][j];
	cand_sort = candidate;
	sort( cand_sort.begin(), cand_sort.end(), comparator );


	size_t idx=0;
	while( true ){
		it1 = find( init.begin(), init.end(), cand_sort[idx].first );
		if( it1==init.end() ){
			ans.push_back( cand_sort[idx].first );
			break;
		}
		idx++;
	}


	while( ans.size()<100 ){
		for( size_t j=0; j<edge[ cand_sort[idx].first-1 ].size(); ++j )
			candidate[j].second += edge[ cand_sort[idx].first-1 ][j];
		cand_sort = candidate;
		sort( cand_sort.begin(), cand_sort.end(), comparator );
		idx=0;
		while( true ){
			it1 = find( init.begin(), init.end(), cand_sort[idx].first );
			it2 = find( ans.begin(), ans.end(), cand_sort[idx].first );
			if( it1==init.end() && it2==ans.end() ){
				ans.push_back( cand_sort[idx].first );
				break;
			}
			idx++;
		}

	}

}
