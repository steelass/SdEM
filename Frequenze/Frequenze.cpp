#include <cstdint>
#include <vector>
#include <iostream>
#include <string>
#include <fstream>
#include <map>
#include <iterator>
#include <algorithm>

#include "frequenze.h"
;using namespace std;

/*class frequency {
	map <char,int> coppie;
	float Entropy;

	public:
		frequency(size_t i): Entropy(0)  {
			char x = 0;
			for (size_t i = 0; i < 256; ++i) {
				coppie[x] = 0;
				++x;				
			}	
		}

		void add(char c) {
			coppie[c]++;
		}

		const size_t dim() {
			return coppie.size();
		}

		int at(char c) {
			return coppie[c];
		}

		auto begin() -> decltype(coppie.begin()) {return coppie.begin();}
		auto begin() const -> decltype(coppie.begin()) {return coppie.begin();}

		auto end() -> decltype(coppie.end()) {return coppie.end();}
		auto end() const -> decltype(coppie.end()) {return coppie.end();}	
};*/

bool compare(pair<char,int> a,pair<char,int> b) {
	return a.second > b.second;
}

int main(int argc, char**argv) {
	cout << "FREQUENZE\n";
	if(argc < 3) {
		cout << "Errore numero parametri!\n";
		return EXIT_FAILURE;
	}
	ifstream in(argv[1],ios::binary);
	ofstream out(argv[2],ios::binary);
	if(!in || !out) {
		cout << "Errore apertura stream\n";
		return EXIT_FAILURE;
	}
	frequency f(256);
	char c;
	
	while(!in.eof()) {
		
		in.get(c);
		if(in.eof()) {
			break;
		}
		//cout << c <<"\n";
		f.add(c);

	}
	cout << "fine estrazioni caratteri\n";
	vector <pair<char,int>> v(0);
	for (std::map<char,int>::iterator it=f.begin(); it!=f.end(); ++it) {
		cout << it->first << " => " << it->second << '\n';
		if(it->second > 0) {
			
			v.push_back(make_pair(it->first,it->second));
		}
	}
	sort(v.begin(),v.end(), compare);
	
	cout << "VETTORE ORDINATO\n";
	for(auto& x : v) {
		cout << x.first << " " << x.second << "\n";
		out << x.first << " " << x.second << "\n";
	}

	//getchar();
	//getchar();

}

/*NOTE:

*/