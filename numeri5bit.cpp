/*
Si scriva un programma numeri5bit in linguaggio C++, a linea di comando, 
che accetti come primo parametro il nome di un file nel formato indicato e come secondo parametro il nome di un file di output.
Il programma deve leggere il file in ingresso e salvare un file binario 
contenente il numero di valori presenti nel file originale, codificato a 32 bit in Big Endian,
seguito da tutti i valori, codificati ognuno con 5 bit. 
Riempire l’ultimo byte con un padding di zeri, se il numero di bit scritti non è multiplo di 8.
*/
#include <cstdint>
#include <iostream>
#include <fstream>
#include <vector>

#include "bitwriter.h"

using namespace std;

/*class bitwriter {
	ostream& _out;
	uint8_t _buf;
	int _n;

public:
	bitwriter(ostream &out, int n=0) : _out(out) , _n(n) {}
	//quando viene eliminato l'oggetto devo finire di scrivere il buffer se c'è qualcosa dentro
	~bitwriter() {
		flush();
	}

	

	//finchè n è maggiore di zero esegue la scrittura nel buffer del bit preso dall'elelemtno spostato di n
	void operator()(uint32_t el,int n){
		while(n-- > 0){
			write(el >> n);
		}

	}
	//scrive sul buffer l'ultimo bit, aumenta n e controlla se è da scrivere il buffer nello stream
	void write(uint32_t bit){
		_buf = (_buf << 1) | (bit & 1);
		_n++;
		if(_n==8){
			_out.put(_buf);
			_n=0;
		}

	}
	//finchè _n > 0 scrive sul buffer degli zeri
	//quando _n arriva a 8 scrive tutto il buffer sullo stream
	//e si azzera n
	void flush(uint32_t bit = 0){
		while (_n)
			write(bit);
	}


	
};*/



/*conteggio il numero di elementi scorrendo il file*/
uint32_t numberElements(ifstream &in){
	uint32_t dim = 0;
	uint16_t num;
	while(true){
		in >> num;
		//cout << num << "\n";
		++dim;
		if(in.eof())
			break;
		
	}
	return dim;
}
/*leggo il file e metto i numeri che trovo in un vector*/
void readElements(ifstream &in,vector<uint16_t> &v) {
	uint16_t num;
	while(true){
		in >> num;
		//cout << num << "\n";
		v.push_back(num);
		if(in.eof())
			break;
		
	}
}


int main (int argc, char* argv[]) {
	cout << "NUMERI 5 BIT\n\n";
	if(argc<3){
		cout << "Errore numero di parametri: " << argc;
		return EXIT_FAILURE;
	}
	else {
		cout <<"input :"<< argv[1] << "\noutput: "<<argv[2] <<"\n";
		ifstream in(argv[1],ios::binary);
		ofstream out(argv[2],ios::binary);
		if(!in || !out) 
			return EXIT_FAILURE;
		uint32_t dim = numberElements(in);
		cout << "Dimensione: "<< dim << "\n";

		/*PER ORA RILEGGO TUTTO E METTO GLI ELEMENTI IN UN VETTORE ->DA MODIFICARE*/
		ifstream inn(argv[1],ios::binary);
		vector<uint16_t> v;
		readElements(inn,v);
		
		bitwriter bw(out);
		
		bw(dim,32);
		//bw.writeJack(dim,32);
		for(auto &x : v)
			//bw.writeJack(x,5);
			bw(x,5);
		
		/*
		cout << "Elementi: \n";
		for(auto &x : v)
			cout << x << "\n";*/
		cout << "FINE!";
		
		
		getchar();
		getchar();
		return EXIT_SUCCESS;
	}

}