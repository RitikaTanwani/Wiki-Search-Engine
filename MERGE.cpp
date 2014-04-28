#include<dirent.h>
#include<iostream>
#include<ctype.h>
#include<cstdio>
#include<cstdlib>
#include<algorithm>
#include<vector>
#include<set>
#include<map>
#include<string>
#include<string.h>
#include<cstring>
#include<stack>
#include<queue>
#include<cassert>
#include<iterator>
#include<cmath>
#include<climits>
#include <bitset>
#include <fstream>
#include "rapidxml-1.13/rapidxml.hpp"
using namespace rapidxml;
using namespace std;

#define FOR(i,a,b) for(int i= (int)a; i< (int)b; ++i)
#define REP(i,n) FOR(i,0,n)

#define LL long long int 
#define PII pair<int,int> 
#define PB push_back
#define MP make_pair
#define INF 1000000000
#define mod 1000000007
#define MX 100005

FILE* fd[MX];
FILE* fout;
int fmx= 0;

pair<int,string> rec;

struct Node{
    string word;
    vector< pair<int,string> > records;
    int fno;

};
bool operator<(const Node& i, const Node& j){
    return i.word > j.word;   
}
bool operator>(const Node& i, const Node& j){
    return i.word < j.word;   
}
bool operator==(const Node& i, const Node& j){
    return i.word == j.word;   
}

priority_queue<Node > heap;
Node cur;
void pushNext( int fno ){
    if(fd[fno] == NULL)  return;
    if( fno >= fmx ){
        exit(1);
    }
    char word[10000];
    int docId;
    char c;
    char cat[10000];
   
    Node temp;

    if( fscanf(fd[fno], "%s",word) == EOF ){
        fclose( fd[fno]);
        fd[fno] = NULL;
        return;
    }
  //  printf("%s\n",word);
    c = ' ';
    temp.word = word;
    temp.fno = fno;
    while(c!='\n'){
        if( fscanf(fd[fno], "%d %s",&docId,cat) == EOF) exit(1);
        if( fscanf(fd[fno], "%c",&c) == EOF ) exit(1);
        string tmpstr = cat ;
    //    rec = make_pair(docId,tmpstr); 
//        temp.records.PB( rec );
        temp.records.PB( make_pair(docId,tmpstr) );
    }
    heap.push(temp);
}

map <int , string> posting; //used to store postings for a single word then emptied.
map <int , string>::iterator itposting;
void buildInd(){
    for(int i=0;i<fmx;i++){
        pushNext( i );
    }
    int i,j,k;
    while(!heap.empty()){
      fflush(fout);
        fflush(stdout);
        k = 0;
        posting.clear();
        cur = heap.top();
        heap.pop();
        for(i=0;i<cur.records.size();i++){
               posting[cur.records[i].first] = cur.records[i].second;
        }
        pushNext( cur.fno );
        while( !heap.empty() && heap.top().word == cur.word ){
            cur = heap.top();
            heap.pop();
            for(i=0;i<cur.records.size();i++){
               posting[cur.records[i].first] = cur.records[i].second;
            }
            pushNext( cur.fno );
        }
        fprintf(fout,"%s",cur.word.c_str());
        for(itposting= posting.begin() ; itposting != posting.end() ; itposting++){
            fprintf(fout," %d %s",itposting->first, itposting->second.c_str());
        }
        fprintf(fout, "\n");
    }
}
void listFile(char* dirName){
    string   str;
    DIR *pDIR;
    struct dirent *entry;
    if( pDIR=opendir(dirName) ){
        while(entry = readdir(pDIR)){
            if( strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0){
                string temp = entry->d_name;
                string dir = dirName;
                temp = dir + temp;
                fd[fmx++] = fopen(temp.c_str(),"r");
               
                if( fd[fmx-1]== NULL){
                    printf("Unable to open a file\n");
                    fmx--;
                }
            }
        }
        closedir(pDIR);
    }
}
int main(int argc , char* argv[]){
    if( argc < 3 ) exit(1);
    fout = fopen(argv[2],"w");
    listFile(argv[1]);
    buildInd();
    fclose(fout);
    return 0;
}

