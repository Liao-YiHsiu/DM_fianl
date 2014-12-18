#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <ctime>
#include <iostream>
using namespace std;

int date2Int(string& date){
   int y, m, d;
   sscanf(date.c_str(), "%d/%d/%d", &y, &m, &d);
   cout << y << "\t" << m << "\t" << d << endl;
   struct tm t1 = {0, 0, 0, d, m-1, y-1900};
   struct tm t2 = {0, 0, 0, 1, 0, 100}; 
   time_t time1 = mktime(&t1); 
   time_t time2 = mktime(&t2); 
   return difftime(time1, time2) / (60*60*24) + 0.5;
}
int main(){
   int yy, mm, dd;
   int a, b;
   string s;
   cin >> a >> s >> b;
   cout << date2Int(s) << endl;
   cin >> a >> yy >> mm >> dd >> b;
   cout << yy << endl << mm << endl << dd;
   return 0;
}
