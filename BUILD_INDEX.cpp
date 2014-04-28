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
#define MX 1000005


xml_document<> doc;
xml_node<> * root_node;
FILE* fd;

//FILE* fdmax;
FILE* fdtitle;
struct com{
    int cat[5];
};
extern int stem( char* s, int i, int j);
map <string, map<int,com> > ind;   /// a 3d vector which store count for a particular word and particular doc in a particular field
map <string, map<int,com> >::iterator it;
map<int,com>::iterator it2;
    string stops[] = { 
    "a","able","about","above","according","accordingly","across","actually","after","afterwards","again","against","aint","all","allow","allows","almost","alone","along","already","also","although","always","am","among","amongst","an","and","another","any","anybody","anyhow","anyone","anything","anyway","anyways","anywhere","apart","appear","appreciate","appropriate","are","arent","around","as","aside","ask","asking","associated","at","available","away","awfully","b","be","became","because","become","becomes","becoming","been","before","beforehand","behind","being","believe","below","beside","besides","best","better","between","beyond","both","brief","but","by","c","came","can","cannot","cant","cause","causes","certain","certainly","changes","clearly","cmon","co","com","come","comes","concerning","consequently","consider","considering","contain","containing","contains","corresponding","could","couldnt","course","cs","currently","d","definitely","described","despite","did","didnt","different","do","does","doesnt","doing","done","dont","down","downwards","during","e","each","edu","eg","eight","either","else","elsewhere","enough","entirely","especially","et","etc","even","ever","every","everybody","everyone","everything","everywhere","ex","exactly","example","except","f","far","few","fifth","first","five","followed","following","follows","for","former","formerly","forth","four","from","further","furthermore","g","get","gets","getting","given","gives","go","goes","going","gone","got","gotten","greetings","h","had","hadnt","happens","hardly","has","hasnt","have","havent","having","he","hello","help","hence","her","here","hereafter","hereby","herein","heres","hereupon","hers","herself","hes","hi","him","himself","his","hither","hopefully","how","howbeit","however","i","id","ie","if","ignored","ill","im","immediate","in","inasmuch","inc","indeed","indicate","indicated","indicates","inner","insofar","instead","into","inward","is","isnt","it","itd","itll","its","itself","ive","j","just","k","keep","keeps","kept","know","known","knows","l","last","lately","later","latter","latterly","least","less","lest","let","lets","like","liked","likely","little","look","looking","looks","ltd","m","mainly","many","may","maybe","me","mean","meanwhile","merely","might","more","moreover","most","mostly","much","must","my","myself","n","name","namely","nd","near","nearly","necessary","need","needs","neither","never","nevertheless","new","next","nine","no","nobody","non","none","noone","nor","normally","not","nothing","novel","now","nowhere","o","obviously","of","off","often","oh","ok","okay","old","on","once","one","ones","only","onto","or","other","others","otherwise","ought","our","ours","ourselves","out","outside","over","overall","own","p","particular","particularly","per","perhaps","placed","please","plus","possible","presumably","probably","provides","q","que","quite","qv","r","rather","rd","re","really","reasonably","regarding","regardless","regards","relatively","respectively","right","s","said","same","saw","say","saying","says","second","secondly","see","seeing","seem","seemed","seeming","seems","seen","self","selves","sensible","sent","serious","seriously","seven","several","shall","she","should","shouldnt","since","six","so","some","somebody","somehow","someone","something","sometime","sometimes","somewhat","somewhere","soon","sorry","specified","specify","specifying","still","sub","such","sup","sure","t","take","taken","tell","tends","th","than","thank","thanks","thanx","that","thats","the","their","theirs","them","themselves","then","thence","there","thereafter","thereby","therefore","therein","theres","thereupon","these","they","theyd","theyll","theyre","theyve","think","third","this","thorough","thoroughly","those","though","three","through","throughout","thru","thus","to","together","too","took","toward","towards","tried","tries","truly","try","trying","ts","twice","two","u","un","under","unfortunately","unless","unlikely","until","unto","up","upon","us","use","used","useful","uses","using","usually","uucp","v","value","various","very","via","viz","vs","w","want","wants","was","wasnt","way","we","wed","welcome","well","went","were","werent","weve","what","whatever","whats","when","whence","whenever","where","whereafter","whereas","whereby","wherein","wheres","whereupon","wherever","whether","which","while","whither","who","whoever","whole","whom","whos","whose","why","will","willing","wish","with","within","without","wonder","wont","would","wouldnt","x","y","yes","yet","you","youd","youll","your","youre","yours","yourself","yourselves","youve","z","zero"
};


static vector<string> stopWords( stops , stops + 563 );


int  docId;
char word[1000];
int wl = 0;
stack<int> state;
map <int,com>::iterator itmax;
void insert(){
       if( wl <=2 ) return ;   /// IF the worldlength is less than 2 then obv it will be a stop word
       word[wl] = '\0';
       word[ stem(word,0,wl-1)+1 ] = '\0';
       string temp = word;
       if( !binary_search( stopWords.begin() , stopWords.end(),word)){
	//       printf("state.top=%d\n",state.top());
	       /*if(state.top()==3){
		       printf("found 3\n");
	       }*/
           ind[temp][docId].cat[ state.top() ] += 1; 
       }
}
void buildInd(){
    if( fd == NULL ) printf("fd is null\n");
    for( it = ind.begin() ; it!=ind.end() ; it++){
        fprintf(fd, "%s",(*it).first.c_str());
        for( it2 = (*it).second.begin() ; it2 != (*it).second.end();it2++){
            fprintf(fd, " %d ",(*it2).first);
            int *a = ((*it2).second).cat;
            if( a[0] > 0){
                    fprintf(fd,"B%d",a[0]); /// means in body 
            }
            if( a[1] > 0){
                    fprintf(fd,"O%d",a[1]);   // square bracket
            }
            if( a[2] > 0){
                    fprintf(fd,"C%d",a[2]);  /// category
            }
            if( a[3] > 0){
                    fprintf(fd,"I%d",a[3]); // infobox
            }
            if( a[4] > 0){
                    fprintf(fd,"T%d",a[4]); // title
            }
        }
        fprintf(fd,"\n");
    }
    fclose( fd );
    fclose( fdtitle) ;

}
char* c;
char pre;
void flower();
void square();
void square(){
	if(state.top()!=2){
		state.push( 1 );
	}
	else{
		state.push(2);
	}
	pre = ' ';
	wl = 0;
	word[wl] = '\0';
	int first = 1;
	while(*c!='\0'){
		if(( *c >= 'A' && *c <= 'Z' )){
			word[wl++] = *c + 32;
		}
		else if( *c >= 'a' && *c<='z' ){ 
			word[wl++] = *c;
		}
		else if( *c == '\''){}
		else if( *c == ']' &&  pre == ']' ){
			pre = ' ';
			c++;
			state.pop();
			return;
		}   
		else if( *c == '{' && pre == '{'){
			c++;
			flower();
			pre = ' ';
			continue;
		}
		else if( *c == '[' && pre == '['){
			c++;
			square();
			pre = ' ';
			continue;
		}
		else {
			if( wl > 0 ){
				word[wl] = '\0';
				if( first ){
					if(strcmp("category",word)==0){
						state.pop();
						state.push( 2 );
					}
					else{
						insert();
					}
					first = 0;
				}
				else{
					insert();
				}
				wl = 0 ;
				word[wl] = '\0';
			}
		}
		pre = *c;
		c++;
	}
	state.pop();
	return ;
}
void flower(){
	if(state.top()!=3){
		state.push(0);
	}
	else{
		state.push(3);
	}
	pre = ' ';
	wl = 0;
	int first = 1;
	word[wl] = '\0';
	while(*c!='\0'){
		if(( *c >= 'A' && *c <= 'Z' )){
			word[wl++] = *c + 32;
		}
		else if( *c >= 'a' && *c<='z' ){ 
			word[wl++] = *c;
		}
		else if( *c == '\''){}
		else if( *c == '}' &&  pre == '}' ){
			pre = ' ';
			c++;
			state.pop();
			return;
		}   
		else if( *c == '{' && pre == '{'){
			c++;
			flower();
			pre = ' ';
			continue;
		}
		else if( *c == '[' && pre == '['){
			c++;
			square();
			pre = ' ';
			continue;
		}
		else {
			if( wl > 0 ){
				word[wl] = '\0';
				if( first ){
					if(strcmp("infobox",word)==0){
						state.pop();
						state.push(3);
					}
					else{
						insert();
					}
					first = 0;
				}
				else{
					insert();
				}
				wl = 0 ;
				word[wl] = '\0';
			}
		}
		pre = *c;
		c++;
	}
	state.pop();
	return ;
}

void processText( xml_node<>* text_node ){
	c = text_node->value();
	pre = ' ';
	state.push(0);
	word[wl] = '\0';
	while( *c != '\0'){
		if(( *c >= 'A' && *c <= 'Z' )){
			word[wl++] = *c + 32;
		}
		else if( *c >= 'a' && *c<='z' ){ 
			word[wl++] = *c;
		}
		else if( *c == '\''){}
		else if( *c == '{' && pre == '{'){
			c++;
			flower();
			pre = ' ';
			continue;
		}
		else if( *c == '[' && pre == '['){
			c++;
			square();
			pre = ' ';
            continue;
        }
        else{
            if( wl > 0 ){
                insert();
                wl = 0;
                word[wl] = '\0';
            }
        }
        pre = *c;
        c++;
    }
	state.pop();
}
void processTitle( xml_node<>* title_node ){
    c = title_node->value();
    pre = ' ';
    state.push(4);
    word[wl] = '\0';
    while( *c != '\0'){
        if(( *c >= 'A' && *c <= 'Z' )){
            word[wl++] = *c + 32;
        }
        else if( *c >= 'a' && *c<='z' ){ 
            word[wl++] = *c;
        }
        else if( *c == '\''){}
        else{
            if( wl > 0 ){
                insert();
                wl = 0;
                word[wl] = '\0';
            }
        }
        pre = *c;
        c++;
    } 
    if( wl > 0 ){
                insert();
                wl = 0;
                word[wl] = '\0';
    }
    state.pop();
}

int main(int argc , char* argv[]){
    if( argc < 2 ) exit(1);
    ifstream theFile(argv[1]);
    vector<char> buffer((istreambuf_iterator<char>(theFile)), istreambuf_iterator<char>());
    buffer.push_back('\0');
    argv[1] = argv[1]+22;
    string outfile = argv[1];

    outfile = "./INDEXES_FOLDER/" + outfile + ".index";
    
    fd = fopen(outfile.c_str(),"w");
    if( fd == NULL) {
        printf("Cannot open file for writing \n");
    }
   string titlefile = "./titleind";
   fdtitle = fopen( titlefile.c_str() , "a");
   
   doc.parse<0>(&buffer[0]);

    root_node = doc.first_node("file");
    // Iterate over the pages
    for(xml_node<> * page_node = root_node->first_node("page"); page_node; page_node = page_node->next_sibling()){
        // Interate over the ids
        xml_node<> * text_node = page_node->first_node("text");
        xml_node<> * title_node = page_node->first_node("title");
        xml_node<> * id_node = page_node->first_node("id");
        docId = atoi(id_node->value());
        if( text_node == NULL) { 
            xml_node<> * revision_node = page_node->first_node("revision");
            text_node = revision_node->first_node("text");
        }
        if( title_node !=NULL ){ 
            fprintf(fdtitle,"%d %s\n",docId,title_node->value());  // storing the title corresponding to a document.
            processTitle( title_node );
        }
        if( text_node != NULL )processText( text_node );
    }
    buildInd();
    return 0;
}
