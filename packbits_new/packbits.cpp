/*
Si scriva un programma in linguaggio C++, a linea di comando, che accetti la seguente sintassi:
packbits c <input file> <output file>
packbits d <input file> <output file>
Quando viene specificata l’opzione “c” il programma apre il file specificato 
(il file deve essere trattato come file binario, ovvero può contenere un valore qualsiasi da 0 a 255 in ogni byte), 
lo comprime con l’algoritmo Packbits e lo salva in un nuovo file (utilizzate per chiarezza l’estensione “.pkb”). 
Quando viene specificata l’opzione “d”, il programma tenta di decomprime il contenuto del file di input salvandolo nel file di output.
*/

/*
Come ulteriore estensione, si realizzi il programma che nel caso ci sia una copia di byte, seguita da una run da 2 byte, 
ancora seguita da una copia di byte, non generi la run, ma la incorpori in un’unica copia.*/
#include <cstdint>
#include <fstream>
#include <iostream>
#include <vector>

#include "packbit.h"

using namespace std;
/*
class packbits {
	vector<uint8_t> buf;
	bool rip;

public:
	packbits() : rip(false), buf(0) {}
	packbits(bool r) : rip(r), buf(0) {}

	void push(uint8_t c) {
		buf.push_back(c);
	}

	const vector<uint8_t>& getBuf() {
		return buf;
	}


	const bool& getRip() {
		return rip;
	}

	void setRip(bool f){
		rip = f;
	}

	uint8_t& operator [] (size_t pos) {
		return buf[pos];
	}

	uint8_t& at(size_t pos) {
		return buf[pos];
	}

	uint8_t& operator () (size_t pos) {
		return buf[pos];
	}

	const uint8_t size() {
		return buf.size();
	}

	void clear() {
		buf.clear();
	}

	bool readfile(ifstream &in,ofstream &out) {
	
	bool ripetizione = false;
	uint8_t c;
	while(!in.eof()) {
		
		if(in.eof()) {
			break;
		}

		in >> c;
		//if(!in.eof()){
			//cout << c << " ";
			if(size() == 0) {
				push(c);
			}
			//size > 0
			else {
				if (size()==1) {
					push(c);
					setRip(c == at(0));
				}
				else { //size > 1
					if (size()>=2) { //size > 2
						if(getRip()) {
						//se il carattere è uguale
							if(c==buf[size()-1]) {
								setRip(true);
								push(c);
							}
						//se il carattere è diverso
						else {
							write(out);
							setRip(false);
							push(c);
							}
						}//fine rip
						else {//non ripetizione
						//se il carattere è uguale
							if(c==buf[size()-1]) {
								write(out);
								push(c);
								setRip(true);
							}
							//se il carattere è diverso
							else {
								push(c);
								setRip(false);
							}
						}//fine non ripetizione
					} //fine size > 2
					if((size()==128 && getRip())
						|| size()==129 && !getRip()) { 
						//se ho una dimensione di 128 e sono in ripetizione ->L = 129
						//se ho una dimensione di 129 e non sono in ripetizione -> L =127
						write(out);
					}

				} //fine size > 1
			} //fine size > 0
		//}	
	}
	if (size() > 0) {
		write(out);
	}
	int fine = 128;
	cout << fine;
	out << fine;
	return true;

	}
	void write(ofstream& out,bool evo=false) {
		//se sono in ripetizione dimensione e carattere che si ripete
		if(getRip()) {
			out << 256- size() + 1<< at(0); 
			cout << 256 - size() + 1<< at(0) << " "; 
			
		}
		//se non sono in ripetizione stampo il buffer meno l'ultimo carattere
		else{
			int meno = 2;
			if(evo) {
				meno = 1;
			}

			cout << size()-meno;
			out << size()-meno; // meno 2 perchè l'ultimo cararttere nel buf non lo stampo e perchè packbits decrementa di 1
			for ( auto &x : getBuf()) {
				if ((x != at(size()-1) && !evo) || evo) {
					out << x;
					cout << x << " ";
				}

			}
		}
		clear();
	}

	void decompress (ifstream& in, ofstream& out) {
		
		uint8_t l;
		int c;
		while(!in.eof()){
	
			in >> c;
			if(c >= 0 && c <=127) {
				c++;
				while(c-->0) {
					l = in.get();
					out << l;
				}

			}
			if(c >= 129 && c <=255) {
				c = 257 - c;
				l = in.get();
				while(c-->0) {
				
					out << l;
				}
			}
			if(in.eof()) {
				break;
			}
		}
	}//fine decompress

	//--------------------------------------------------------
	//PARTE 2

	bool readfileEvo(ifstream &in,ofstream &out) {
	uint8_t contR = 0;
	uint8_t contS = 0;
	bool ripetizione = false;
	uint8_t c;
	while(!in.eof()) {
		in >> c;
		if(in.eof()){
				break;
			}
		//if(!in.eof()){
			//cout << c << " ";
			if(size() == 0) {
				push(c);
				++contS;
			}
			//size > 0
			else {
				if (size()==1) {
					push(c);
					setRip(c == at(0));
					getRip() ? contR=2 : contS = 2;
				}
				else { //size > 1
					if (size()>=2) { //size > 2
						if(getRip()) {
						//se il carattere è uguale
							if(c==buf[size()-1]) {
								setRip(true);
								push(c);
								++contR;
							}
						//se il carattere è diverso
						else {
							if(contR>2){
								write(out);								
							}
							else {
								contS=contR + 1;
							}
							contR = 0;
							setRip(false);
							push(c);
						
							}
						}//fine rip

						else {//non ripetizione
						//se il carattere è uguale
							if(c==buf[size()-1]) {
								if(contS>3){
									write(out);
									contS = 0;
								}
								else{
									contR = 2;
									++contS;
								}
								push(c);
								//setRip(true);
							}
							//se il carattere è diverso
							else {
								if(contR == 2) {
									setRip(false);
									contR = 0;
								}

								push(c);
								++contS;
								setRip(false);
							}
						}//fine non ripetizione
					} //fine size > 2
					if((size()==128 && getRip())
						|| size()==129 && !getRip()) { 
						//se ho una dimensione di 128 e sono in ripetizione ->L = 129
						//se ho una dimensione di 129 e non sono in ripetizione -> L =127
						write(out,true);
						contS=0;
						contR = 0;
					}

				} //fine size > 1
			} //fine size > 0
		//}	
			

	}
	if (size() > 0) {
		write(out,true);
	}
	int fine = 128;
	cout << fine;
	out << fine;
	return true;

	}//fine readfileEVO
};

*/



int main(int argc, char **argv) {
	cout << "PACKBITS\n";
	if (argc <= 3) {
		cout << "Errore numero parametri\n";
		getchar();
		getchar();
		return EXIT_FAILURE;
	}
	
	if (string(argv[1]).compare("c")==0) {
		ifstream in(argv[2],ios::binary);
		ofstream out (argv[3],ios::binary);
		ifstream in1(argv[4],ios::binary);
		ofstream out1 (argv[5],ios::binary);
		if(!in || !out || !in1 || !out1) {
			cout << "Errore apertura stream di input/output\n";
			getchar();
			getchar();
			return EXIT_FAILURE;
		}
		cout << "Compressione\n";
		packbits p;
		//p.readfile(in,out);
		cout << "EVO\n";
		p.readfileEvo(in1,out1);
	}
	else {
		cout << "Decompressione\n";
		ifstream in(argv[3],ios::binary);
		ofstream out (argv[2],ios::binary);
		ifstream in1(argv[5],ios::binary);
		ofstream out1 (argv[4],ios::binary);
		if(!in || !out || !in1 || !out1) {
			cout << "Errore apertura stream di input/output\n";
			getchar();
			getchar();
			return EXIT_FAILURE;
		}
		packbits p;
		//p.decompress(in,out);
		p.decompress(in1,out1);
	}
	getchar();
	getchar();
	return EXIT_SUCCESS;
}
