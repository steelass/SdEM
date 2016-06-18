#include <iostream>
#include <fstream>
#include <vector>
#include <iterator>
#include <algorithm>
#include <string>
#include <functional>
#include <exception>
#include <queue>
#include <cstdint>

using namespace std;

#include "bitstreams.h"

class vlc{		
	uint8_t len;
	int code;
	string tag;

public :
	vlc(uint8_t l, string s) : len(l),tag(s),code(0){} 
	uint8_t Len() {
		return len;
	}
	string Tag(){
		return tag;
	}

	int Code() {
		return code;
	}


	void setCode(int c) {
		code = c;
	}

	void setLen(uint8_t l) {
		len = l;
	}

	void setTag(string t) {
		tag = t;
	}




};

void ExtractTable(ifstream &in,vector<vlc> &v, bitreader &br) {
	uint16_t N = 0; //numero di ricorrenze nella tabella
	uint8_t cod; //tag
	
	char c;
	uint8_t len ; //lunghezza da prendere
	cout << "Lettura numero simboli tabella\n";
	size_t y = 0;
	while(y < 16) {
		N = N << 1 | br(1);
		y++;
	}

	string s("");
	cout << N << "\nLettura simboli\n";
	size_t zeros; //numeri di zeri
	size_t cont = 0; //contatore per il numero di ricorrenze nella tabella
	size_t nbit;
	while(cont < N){
		
		s.clear();
		cod = 0;
		zeros = 0;
		nbit = 0;
		len = 0;
		do{		
			//in >> el;
			in.get(c);
			s.push_back(c);
		}while(c!=0);
		
		size_t x = 0;
		len = br(8);

		++cont;
		vlc tag(len,s);
		v.push_back(tag);
		cout << int(cont) << ": parola->" << s << "|lunghezza->"<<int(len) << "\n"; 
	}

	cout << "Fine lettura tabella\n";	


}

bool compare(vlc a, vlc b) {
	//if(a.Len()!= b.Len())
		return a.Len() < b.Len();
	
}

void CreateHuff(vector<vlc> &v){
	int code = 0;
	uint8_t len = 0;
	
	for(size_t i = 0; i< v.size(); ++i) {
		while(len < v[i].Len()){
			++len;
			code = code << 1;
		}
		v[i].setCode(code);
		code++;


	}

}

bool checkHuff(ifstream &in, vector<vlc> v, vlc &el, bitreader &br){
	
	uint8_t len = 0;
	int code = 0;
	bool found = false;
	for(size_t i = 0; i < v.size() && !found; ++i) {
		while(len < v[i].Len()) {
			len++;
			code = code << 1 | br(1);
		}
		if(code==v[i].Code()) {
			found = true;
			el.setCode(code);
			el.setLen(len);
			el.setTag(v[i].Tag());
		}

	}
	return found;
}

void addElement(ifstream &in, ofstream &out, vector<vlc> &v, bitreader &br){
	//trovo la stringa corrispondente al codice di huffman
		vlc tag(0,"");
		uint8_t n_attr = 0;
		size_t i = 0;
		checkHuff(in,v ,tag, br);
		out << "<" << tag.Tag();
		cout << "<" << tag.Tag();
		//trovo il numero di attibuti
		n_attr = br(4);

		//per n_attr volte cerco le coppie nome attributo/valore attributo
		i = 0;
		vlc nome_attr(0,"");
		vlc val_attr(0,"");
		while(i<n_attr) {
			checkHuff(in,v,nome_attr,br);
			out <<" "<< nome_attr.Tag() <<"=";
			cout <<" "<< nome_attr.Tag() <<"=";
			checkHuff(in,v,val_attr,br);
			out << "\"" <<val_attr.Tag() <<"\" ";
			cout << "\"" <<val_attr.Tag() <<"\" ";
			++i;
		}

		out << "?>\n";
		cout << "?>\n";
		//trovo se ha figli o dati;
		bool hasChildren;
		hasChildren = br(1);

		//se ha figli
		if(hasChildren) {
			//trovo il numero di figli
			size_t j = 0;
			uint16_t n_children = 0;
			n_children = br(10);
		
			//per n_children volte eseguo questa funzione di nuovo
			while(j < n_children) {
				addElement(in,out,v,br);
				j++;
			}

		//se ha dati
		}else {
			//trovo il numero di byte usati
			size_t j = 0;
			uint16_t n_byte = 0;
			n_byte = br(10);
			
			char c;
			//per n_byte volte butto fuori 1 byte
			while(j<n_byte) {
				c = br(8);
				out<< c;
				cout<< c;
				++j;
			}
			
		}
		out << "\n</"<<tag.Tag()<<">\n";
	cout << "\n</"<<tag.Tag()<<">\n";
	
}


void toXML(ifstream &in, ofstream &out, vector<vlc> &v, bitreader &br){
	out << "<?xml version \"1.0\" encoding=\"UTF-8\"?>\n";
	cout << "<?xml version \"1.0\" encoding=\"UTF-8\"?>\n";
	addElement(in,out,v,br);
	//chiudo il tag
	
}


int decompress(string sInputFileName, string sOutputFileName) {
	// Apertura file di input
	
	cout << "Apertura stream\n";
	ifstream in(sInputFileName, ios::binary);
	ofstream out(sOutputFileName, ios::binary);
	if(!in||!out) {
		cout << "Errore apertura stream\n";
		return EXIT_FAILURE;
	}
	// Estrazione tabella delle stringhe
	bitreader br(in);
	cout << "Estrazione tabelle\n";
	vector <vlc> v;
	ExtractTable(in,v,br);
	cout << "Ordino la tabella\n";
	stable_sort(v.begin(),v.end(),compare);
	// Ricostruzione dei codici di Huffman
	cout << "Ricostruisco il codice di Huffman\n";
	CreateHuff(v);
	// Ricostruzione della struttura dell'XML
	cout << "Ricostruisco XML\n";
	
		toXML(in,out,v,br);
		
	
	// Output dell'XML

	return EXIT_SUCCESS;
}

int main(int argc, char *argv[]) {
	if(argc!=3) {
		cout<< "Numero parametri errato\n";
		return EXIT_FAILURE;
	}

	// TODO : gestire la linea di comando

	string sInput(argv[1]);
	string sOutput(argv[2]);
	if (!sInput.find(".cmp")) {
		cout << "Input sbagliato\n";
		return EXIT_FAILURE;
	}
	if(!sOutput.find(".xml")){
		cout << "Input sbagliato\n";
		return EXIT_FAILURE;
	}


	if(decompress(sInput,sOutput)==0)
		return EXIT_SUCCESS;
	else
		return EXIT_FAILURE;
}
