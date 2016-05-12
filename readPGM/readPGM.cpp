/*
Si scriva un programma che apra il file “rana_asc.pgm” e ne crei una versione “a testa in giù”, ovvero la
prima riga in alto diventi l’ultima in basso della nuova immagine, la seconda diventi la penultima e così
via. Si salvi l’immagine nei formati PGM e plain PGM semplificati, descritti precedentemente. Verificare
che l’immagine sia visualizzabile in XnView.
*/

#include <cstdint>
#include <vector>
#include <iostream>
#include <fstream>
#include "image.h"

;using namespace std;


void reverse(size_t h, size_t l,vector<uint8_t> &origin) {
	vector<uint8_t> dest;
	for(size_t y = h-1; y > 0; y--) {
		for (size_t x = 0; x < l; ++x) {
			dest.push_back(origin.at(y * l + x));
		}
	}
	for (size_t x = 0; x < l; ++x) {
		dest.push_back(origin.at(0 * l + x));
	}
	origin.swap(dest);
	dest.clear();
}


int main(int argc, char *argv[]) {

	if(argc<6) {
		cout << "Passare almeno un parametro!\n";
		return EXIT_FAILURE;
	}

	cout << "...Apertura stream di input e output...\n";
	ifstream in(argv[1],ios::binary);
	ofstream out(argv[2],ios::binary);
	ofstream out_p(argv[3],ios::binary);
	ifstream in2(argv[4],ios::binary);
	ofstream out2(argv[5],ios::binary);
	ofstream out_p2(argv[6],ios::binary);
	if(!in || !out || ! out_p || !in || !out2 || !out_p2){
		cout << "Errore apertura file di input/output!\n";
		getchar();
		getchar();
		return EXIT_FAILURE;
	}
	cout << "RANA_ASC PGM\n";
	int l = 0,h = 0;
	vector<uint8_t> v;

	cout << "...Lettura immagine...\n";
	string descr(readPGM(in,l,h,v,true));
	if (descr.compare("")==0) {
		return EXIT_FAILURE;
	}

	cout << "...reverse...\n";
	imagePGM i(h,l,descr);
	reverse(h,l,v);
	i.setMatrix(v);

	cout << "...scrittura immagine...\n";
	i.writePGM(out,false);

	cout << "...scrittura immagine plain ...\n";
	i.writePGM(out_p,true);

	cout << "RANA_BIN PGM\n";
	v.clear();
	string descr1(readPGM(in2,l,h,v,false));
	if (descr1.compare("")==0) {
		return EXIT_FAILURE;
	}

	cout << "...reverse...\n";
	imagePGM ii(h,l,descr1);
	reverse(h,l,v);
	i.setMatrix(v);

	cout << "...scrittura immagine...\n";
	i.writePGM(out2,false);

	cout << "...scrittura immagine plain ...\n";
	i.writePGM(out_p2,true);
	return EXIT_SUCCESS;
}

/*
NOTE:
per fare sotto classe devo fare classe2 : public classe1 {..}
per il costruttore devo invocare il costruttore della classe genitore per i membri definiti al suo interno

per la lettura binaria:
			int pos = in.tellg(); //POSIZIONE CORRENTE
			in.seekg (pos, in.end); //MI SPOSTO DALLA POSIZIONE CORRENTE ALLA FINE
			int length = in.tellg(); //PRENDO LA LUNGHEZZA
			in.seekg (pos, 0); //RITORNO NELLA POSIZIONE
			v.resize(length); //RIDIMENSIONO IL VETTORE
			in.read(reinterpret_cast<char*>(&v[0]),length*sizeof(uint8_t)); //ESEGUO LA READ
per la lettura in plain uso gli stream

*/
