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



void makeGradient(image& img) {
	for(size_t i = 0; i < img.h() ; ++i) {
		for(size_t j = 0; j < img.l() ; ++j) {
			img(j,i) = i;
		}
	}

}


int main(int argc, char* argv[]){
	cout << "GRADIENT PGM\n";
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
	image i(H,L);
	makeGradient(i);
	i.writePGM(out);
	i.writePGM(out_p,true);

	return EXIT_SUCCESS;


}
