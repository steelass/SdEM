#include <vector>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <iterator>
#include <numeric>

using namespace std; 

bool make_freq(const string& input, const string &output, vector<size_t> &v) {
	/* apertura stream*/
	ifstream in(input,ios::binary);
	ofstream out(output,ios::binary);
	
	if(!in || !out) 
		return false;
	in.unsetf(ios::skipws);

	/*lettura caratteri con unsigned char*/
	uint8_t c;
	do {
		in >> c;
		++v[c];

	}while(!in.eof());
	
	/*creazione struttura dati composta dalla coppia
	uint8_t, numero ripetizioni*/
	vector<pair<uint8_t,size_t>> f(256);
	uint8_t u = 0;
	for(auto& x : v) {
		f.push_back(make_pair(u,x));
		++u;
	}
	/* ordinamento per numero di ripetizioni, dal maggiore al minore*/
	sort(f.begin(),f.end(),
		[](const pair<uint8_t,size_t> & a,const pair<uint8_t,size_t> & b){
			return a.second>b.second;});

	/*scrittura sullo stream di output*/
	for(auto& x : f) {
		if( x.second!= 0){
			if (x.first < 32 || x.first >= 128)
				out << int(x.first);
			else if (x.first == '\"')
			out << "\"\"\"\"";
		else
			out << "\"" << x.first << "\"";
		out << "\t" << x.second << "\n";
		}

		
	}
	return true;
}

double log2( double n )  
{  
    // log(n)/log(2) is log2.  
    return log( n ) / log( 2 );  
}
/*calcolo entropia*/
double entropy(vector<size_t> &v) {
		double tot = 0.0;
		for (const auto& x : v)
			tot += x;

		double H = 0.0;
		for (const auto& x : v) {
			double p = x / tot;
			H += -p* log2 (p);
		}
		return H;
	}

int main(int argc,char**argv){
	
	/*
	cout << "--------------\n";
	cout << "Parametri:\n
	cout << argv[1] << "\n";
	cout << argv[2] << "\n";
	cout << argv[3] << "\n";
	cout << "--------------\n";
	*/

	string type(argv[1]);
	string input(argv[2]);
	string output(argv[3]);

	if(type.compare("c")==0){
		cout << "...Creazione delle frequenze...\n";
		/*inizializzo il vettore per i simboli ASCII*/
		vector<size_t> frequenze(256,0);
		/*calcolo le frequenze*/		
		make_freq(input,output,frequenze);
		/*calcolo l'entropia*/
		cout << "Entropia: " << entropy(frequenze) << "\n";
		return EXIT_SUCCESS;
	}
	else
		return EXIT_FAILURE;
}