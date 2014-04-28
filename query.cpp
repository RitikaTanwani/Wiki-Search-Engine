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

FILE* fout;
FILE* fin;
FILE* f1, *f2 ,*f3;
FILE* fdmax1, *fdmax2;
FILE* fdtitle1,*fdtitle2;
struct docnode{
	int docId;
	double w;
	int count;
};
typedef struct docnode docnode;
bool operator<(docnode i,docnode j){
	return i.docId < j.docId;
}

FILE* ff;
map < string, long long int > idx;  // for storing the secondary index

map < string, long long int >::iterator itidx;

map <string,char> query;  /// for storing in which filed we have to look for the query.

map <string,char>::iterator itq;

map <string, vector< docnode > > wordpostings;  /// posting list for each word of the query.

map <string, vector< docnode > >::iterator itwp;

priority_queue< docnode > heap;

vector< docnode> results;

vector< docnode>::iterator itresults;

#define QMX 1000

extern int stem( char* s, int i, int j);

void multiLevelInd(char* infile, char* outfile , int gaps){
    fin = fopen(infile,"r");
    fout = fopen(outfile , "w");
    char word[1000];
    LL pos;
    int i;
    char c;
    int docId;
    int counter = 0;
    char cat[1000];
    while(1){
        pos = ftell( fin );
        if( fscanf(fin , "%s",word) == EOF ) break;
        while(1){
            fscanf(fin,"%c",&c);
            if( c == '\n') break;
            if( fscanf(fin,"%s",cat) == EOF) break;
        }
        if(counter%gaps==0) fprintf(fout,"%s %lld\n",word , pos ); 
        counter++;
    }
    fprintf(fout,"%s %lld\n","{}",pos);
    fclose(fin);
    fclose(fout);
}
void multiLevelInd2(char* infile, char* outfile , int gaps){
    fin = fopen(infile,"r");
    fout = fopen(outfile , "w");
    char word[1000];
    LL pos;
    LL temp;
    int i;
    char c;
    int counter = 0;
    while(1){
        pos = ftell( fin );
        if( fscanf(fin,"%s",word) == EOF ) break;
        fscanf(fin,"%lld",&temp);
        fscanf(fin,"%c",&c);
        if(counter%gaps==0) fprintf(fout,"%s %lld\n",word , pos ); 
        counter++;
    }
    fprintf(fout,"%s %lld\n","{}",pos);
    fclose(fin);
    fclose(fout);
}
void loadIndexInMem( char* ind ){
    fin = fopen(ind,"r");
    char word[1000];
    LL pos;
    int i;
    char c;
    while(1){
        if( fscanf(fin,"%s",word) == EOF ) break;
        fscanf(fin,"%lld",&pos);
        string tword = word;
        idx[tword] = pos;
    }
    fclose(fin);
}
void search( string w ){
        itidx = idx.lower_bound( w );
        LL pos = -1;
        LL temppos;
        char word[1000] = {'\0'};
        if( itidx->first == w ){
            fseek( f2, itidx->second , SEEK_SET);
            fscanf(f2,"%s",word);
            fscanf(f2,"%lld",&pos);
        }
        else{
            if( itidx == idx.begin()){
            }
            else{
                itidx--;
                fseek( f2, itidx->second , SEEK_SET);
                while(1){

                    if( fscanf(f2,"%s",word) == EOF ) break;
                    fscanf(f2,"%lld",&temppos);
                    int flag = strcmp( word , w.c_str());
                    if( flag > 0 ) break;
                    if( flag == 0 ){
                        pos = temppos;
                        break;
                    }
                }   
            }
        }
        if( pos != -1 ){
            vector<docnode> postings;    
            fseek( f3 , pos , SEEK_SET );
            fscanf(f3 , "%s",word);
            int docId;
            char c;
            while( 1 ){
                char cat[100];
                fscanf(f3,"%d",&docId);
                fscanf(f3,"%s",cat);
                fscanf(f3,"%c",&c);
		char* p = cat;
		int freq=0;
		if(query[w]=='n'){
			freq=0;
			while(*p!='\0'){
				p++;
				while( *p !='\0' && *p >='0' && *p<='9'){
					freq = 10*freq + *p - 48;
					p++;
				}
			}
			docnode docpair;
			docpair.docId  = docId ;
			docpair.w = (double ) freq;
			docpair.count=1;
			postings.PB( docpair );

		}
		else{

			while( *p != '\0' ){
				if( (*p+32) == query[w] ){
					freq = 0;
					p++;
					while( *p !='\0' && *p >='0' && *p<='9'){
						freq = 10*freq + *p - 48;
						p++;
					}
					if( query[w] == 't' ){
						freq *= 20;   
					}
					else if( query[w] == 'o'){
						freq *= 5;   

					}
					else if( query[w] == 'i'){
						freq *= 15;   

					}
					else if( query[w] == 'c'){
						freq *= 10;   
					}
					docnode docpair;
					docpair.docId  = docId ;
					docpair.w = (double ) freq;
					docpair.count=1;
					postings.PB( docpair );
				}
				else p++;
			}
		}
                if( c == '\n') break;
            }
            wordpostings[ w ] = postings;
        }
}
bool prompt(){
    printf("Search $ ");
    char q[QMX];
    if( scanf("%[^\n]",q) == EOF ) return 1;
    getchar();
    char *x=strstr(q,":");
    wordpostings.clear();
    query.clear();
    if(x==NULL){
	    char* pch = strtok (q," ");
	    int i=0;
	    char tchar[1000];
	    query.clear();
	    wordpostings.clear();
	    while(pch != NULL)
	    {   
		    i++;
		    strcpy( tchar , pch );
		    int len = 0;
		    for(char* temp = tchar; *temp!='\0';temp++,len++){
			    if( *temp >='A' && *temp<='Z') *temp += 32;
		    }
		    tchar[ stem( tchar,0,len-1 ) + 1 ] = '\0';
		    string temp = tchar;
		    query[temp] = 'n';
		    pch = strtok (NULL," ");
	    }
    }
    else{
	    char* pch = strtok (q," :");
	    int i=0;
	    char tchar[1000];
	    char attr;
	    query.clear();
	    wordpostings.clear();
	    while(pch != NULL)
	    {   
		    i++;
		    strcpy( tchar , pch );
		    int len = 0;
		    for(char* temp = tchar; *temp!='\0';temp++,len++){
			    if( *temp >='A' && *temp<='Z') *temp += 32;
		    }
		    tchar[ stem( tchar,0,len-1 ) + 1 ] = '\0';
		    if( i&1 ){
			    attr = tchar[0];
			    if( attr == 'b' ) attr = 'b';
		    }
		    else{
			    string temp = tchar;
			    query[temp] = attr;
		    }
		    pch = strtok (NULL," :");
	    }
    }
    return 0;
}
int func(docnode i,docnode j ){
	if(i.count > j.count){
		return 1;
	}
	else if(i.count==j.count){
		if(i.w > j.w ){
			return 1;
		}
		return 0;
	}
	return 0;
}

void mergeDocIds(){
    results.clear();
//    printf("In mere\n");
    for(itwp=wordpostings.begin();itwp!=wordpostings.end();itwp++){
        for(int i=0;i<itwp->second.size();i++){
	//	printf("docid==%d\n",itwp->second[i].docId);
            heap.push(itwp->second[i] );
        }
    }
//    cout << "heap " << endl;
    while( !heap.empty() ){
        docnode cur = heap.top();
//	printf("docid==%d\n",cur.docId);
        heap.pop();
        while( !heap.empty() && heap.top().docId == cur.docId ){
            docnode sub = heap.top();
            heap.pop();
            cur.w += sub.w;
	    cur.count+=sub.count;
        }
        results.PB( cur );
    }
  //  cout << "sorting" << endl;
   /* for(int i=0;i<results.size();i++){
	    cout << results[i].docId << endl;
    }*/
    sort( results.begin() , results.end() , func );
//    cout << "sorting" << endl;
}
void displayTitle( int d){
    char buffer[1000];
    int docId;
    char c;
    fseek ( fdtitle1 , 0 , SEEK_SET);
    while(1){
        if(fscanf(fdtitle1, "%d%c",&docId, &c)== EOF) break;
        fscanf(fdtitle1, "%[^\n]",buffer);
        if( docId == d ) {
            printf("%s\n",buffer);
            break;
        }
    }
}
void displayResults(  ){
    fdtitle1 = fopen("titleind","r");
    int i =0;
    for( itresults = results.begin() ; itresults != results.end() ; itresults++){
        displayTitle( itresults->docId  );
        i++;
        if( i == 15 ) break;
    }   
   fclose( fdtitle1 );
}
/*void printQuery( ){
   for(itq = query.begin(); itq != query.end() ; itq++){
        cout << itq->first << " " << itq->second << endl;
   }
}*/
void getPostings(){
   for(itq = query.begin(); itq != query.end() ; itq++){
        search( itq->first );
   }
}
/*void printPostings(){
    for(itwp=wordpostings.begin();itwp!=wordpostings.end();itwp++){
        cout << itwp->first << ": " ;
        for(int i=0;i<itwp->second.size();i++){
            printf("%d %lf ",itwp->second[i].docId,itwp->second[i].w);
        }   
        printf("\n");
      }
}*/
void processQueries(){
   while(1){
    if( prompt() ) break;
    getPostings();
    mergeDocIds();
    displayResults();
   }
}
int main(int argc , char* argv[]){
//    if( argc < 4 ) exit(1);

    /*multiLevelInd( "finalindex" , "bb" , 1);
    multiLevelInd2( "bb" , "aa" , 100 );
    loadIndexInMem( "aa");
   */ multiLevelInd( argv[3] , argv[2] , 1);
    multiLevelInd2( argv[2] , argv[1] , 100 );
    loadIndexInMem( argv[1] );
    /*multiLevelInd( argv[3] , argv[2] , 1);
    multiLevelInd2( argv[2] , argv[1] , 100 );
    loadIndexInMem( argv[1] );*/
  /*  f3 = fopen( "finalindex" , "r" );
    f2 = fopen( "bb" , "r" );
    f1 = fopen( "aa" , "r" );
    */
    f3 = fopen( argv[3] , "r" );
    f2 = fopen(argv[2] , "r" );
    f1 = fopen( argv[1] , "r" );
  
    processQueries();
    return 0;
}
