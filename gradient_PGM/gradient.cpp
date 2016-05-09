/*
Si scriva un programma che generi un’immagine a livelli di grigio di 256×256 pixel, in cui la prima riga è
costituita di 256 valori 0, la seconda di 256 valori 1, la terza di 256 valori 2 e così via. Si salvi l’immagine
nei formati PGM e plain PGM semplificati, descritti precedentemente. Verificare che l’immagine sia
visualizzabile in XnView. L’immagine dovrebbe apparire come un gradiente dal nero al bianco dall’alto
verso il basso.
*/

#include <cstdint>
#include <vector>
#include <iostream>
#include <fstream>
#include <iterator>

#include "image.h"

;
using namespace std;

int L = 256;
int H = 256;



void makeGradient(image<uint8_t>& img) {
	for(size_t i = 0; i < img.h() ; ++i) {
		for(size_t j = 0; j < img.l() ; ++j) {
			img(j,i) = i;
		}
	}

}


int main(int argc, char* argv[]){
	cout << "GRADIENT PGM\n";
	cout << "...Apertura stream di output...\n";
	if(argc<2) {
		cout << "Passare almeno un parametro!\n";
		return EXIT_FAILURE;
	}
	ofstream out(argv[1],ios::binary);
	ofstream out_p(argv[2],ios::binary);
	if(!out || ! out_p){
		cout << "Errore apertura file di output!\n";
		return EXIT_FAILURE;
	}
	cout << "...Creazione Immagine Gradiente...\n";
	image<uint8_t> i(H,L);
	makeGradient(i);
	cout << "...Scrittura formato PGM...\n";
	i.writePGM(out);
	cout << "...Scrittura formato PGM plain...\n";
	i.writePGM(out_p,true);

	return EXIT_SUCCESS;
}

/*
NOTE:
per la scrittura in plain devo fare una write; poichè devo fare un cast per ogni elemento del vettore matrix,
mi conviene fare una funzione che mi casta il primo elemento ed un altra che mi dice quanti byte devo scrivere.
DEVO USARE LA WRITE PERCHè OGNI LIVELLO DI GRIGIO è RAPPRESENTATO IN BINARIO CON UN BYTE (lo stream era stato
aperto in ios: binary).

Per la scrittura in formato plain, VISTO CHE I VALORI DELLA MATRICE SONO RAPPRESENTATI COME UNA SEQUENZA DI
CARATTERI ASCII SEGUITI DA SPAZIO, POSSO USARE L'OPERATORE >> E UN CAST AD INT OPPURE ITERATORI.
*/
