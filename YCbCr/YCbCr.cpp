#include <cstdint>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include "imagePPM.h"

/*
/*Si scriva un programma a linea di comando, che accetti come primo parametro il nome di un file in formato PPM e come secondo parametro il nome 
di un file di output che sarà in formato PGM.
Il programma deve leggere l’immagine in ingresso, convertirla in memoria in YCbCr, 
dimezzare le dimensioni dei piani Cb e Cr e comporre un’immagine ad un solo canale con Y affiancato a Cb e Cr uno sotto l’altro:YCbCr
Un’immagine RGB 100×100 diventa così un’immagine a livelli di grigio 150×100. 
Se l’immagine ha larghezza o altezza dispari, replicare l’ultima colonna o riga (o entrambe).*/

using namespace std;

class YCbCr {
	uint8_t Y;
	uint8_t Cb;
	uint8_t Cr;

public:
	YCbCr() {}
	YCbCr(uint8_t y,uint8_t cb,uint8_t cr) : Y(y), Cb(cb), Cr(cr) {}

	const uint8_t getY() {
		return Y;
	}
	const uint8_t getCb() {
		return Cb;
	}
	const uint8_t getCr() {
		return Cr;
	}

	void set (uint8_t y,uint8_t cb,uint8_t cr) {
		Y = y;
		Cb = cb;
		Cr = cr;
	}


};


void toYCbCr(vector<RGB> &in, vector<YCbCr> &out) {
	YCbCr el(0,0,0);
	uint8_t Y,Cb,Cr;
	for (auto& x: in) {
		Y = 16 + (1.0/256)*(65.738 * x.getR() + 129.057 * x.getG() + 25.064 * x.getB());
		Cb = 128 + (1.0/256)*(-37.945 * x.getR() - 74.494 * x.getG() + 112.439 * x.getB());
		Cr = 128 + (1.0/256)*(112.439 * x.getR() - 94.154 * x.getG() - 18.285 * x.getB());

		el.set(Y,Cb,Cr);
		out.push_back(el);
	}

}

void DimezzaCbCr(size_t l, size_t h ,vector<YCbCr> &in, imagePGM &iY, imagePGM &iCb, imagePGM &iCr) {
	bool m = false;
	std::vector<YCbCr> v;
	vector<uint8_t> Y;
	vector<uint8_t> Cb;
	vector<uint8_t> Cr;

	v.clear();
	if (l % 2 ) {
		int i = 0;
		for( auto &x : in) {
			v.push_back(x);
			++i;
			if(i%l==0) {
				v.push_back(x);
			}
		}
		++l;
		m = true;
	}

	if (h % 2) {
		std::cout << "Aggiungo colonna\n";
		for( auto &x : in) {
			v.push_back(x);
		}
		for(size_t x = in.size()- l;x < in.size(); ++x) {
			v.push_back(in[x]);
		}
		++h;
		m = true;
	}
	if (!m) {
		for( auto &x : in) {
			v.push_back(x);
		}
	}

	//creo il vettore con solo Y della dimensione giusta e lo setto in unimmagine
	for (auto &x : v) {
		Y.push_back(x.getY());
	}
	iY.setH(h);
	iY.setL(l);
	iY.setMatrix(Y);
	

	//dimezzo il vettore Cb Cr e li aggiungo alle rispettivi immagini
	uint8_t cb,cr;
	for(std::size_t y=0; y < h-1; y+=2) {
		for (std::size_t x = 0; x < l-1; x+=2) {
			cb = (v[y*l+x].getCb()+  v[y*l+x+1].getCb() + v[(y+1)*l+x].getCb() + v[(y+1)*l+x+1].getCb())/4 ;
			cr = (v[y*l+x].getCr()+  v[y*l+x+1].getCr() + v[(y+1)*l+x].getCr() + v[(y+1)*l+x+1].getCr())/4 ;	

			Cb.push_back(cb);
			Cr.push_back(cr);
		}
	}
	iCb.setL(l/2);
	iCb.setH(h/2);
	iCb.setMatrix(Cb);
	
	iCr.setL(l/2);
	iCr.setH(h/2);
	iCr.setMatrix(Cr);
}


void makePGM(imagePGM &iY, imagePGM &iCb, imagePGM &iCr, ofstream &out) {
	vector<uint8_t> v;
	size_t x = 0;
	size_t y = 0;
	bool flag = false;

	for(auto &i : iY.getMatrix() ) {
		//cout << "Y ";
		++x;
		v.push_back(i);
		if (x % iY.l() == 0) {
			++y;
			if(y <= (iY.h() / 2) && !flag){
				size_t j;
				for (j = 0; j < iCb.l(); j++) {
					cout << "Cb "<< y-1<< " - " << j << "\n";
					v.push_back(iCb(j,y-1));
					
				}
			}else {
				if(y==134) {
					y = 1;
					flag = true;
				}

				size_t jj = 0;

				for (jj = 0; jj < iCr.l(); ++jj) {
					cout << "Cr "<< y-1<< " - " << jj << "\n";;
					v.push_back(iCr(jj,y-1));
					
				}
			}
		}
		//cout << "\n";
	}//fine scorrimento y

	imagePGM i(iY.h(), iY.l() + iCb.l(), false);
	i.setMatrix(v);
	i.setDescr("Ce l'abbiamo fatta cazzo!");
	i.writePGM(out,false);
}


int main(int argc, char**argv) {
	cout << "ESERCIZIO 2\n";
	cout << "APERTURA STREAM\n";
	ifstream in("rana.ppm",ios::binary);
	ofstream out("out.pgm",ios::binary);
	ofstream yy("Y.pgm", ios::binary);
	ofstream cbcb("Cb.pgm", ios::binary);
	ofstream crcr("Cr.pgm", ios::binary);
	if(!in || ! out || !yy || !cbcb || !crcr) {
		cout << "Errore apertura stream\n";
		return EXIT_FAILURE;
	}
	cout << "LETTURA IMMAGINE PPM\n";
	size_t h,l;
	string descr;
	vector<RGB> rgb;
	vector<YCbCr> ycbcr;
	string ret(readPPM(in,l,h,descr,rgb));
	imagePPM i(h,l,descr);
	i.setMatrix(rgb);
	cout << "TRASFORMAZIONE IN YCbCr\n";
	toYCbCr(rgb,ycbcr);
	imagePGM iY(0,0),iCb(0,0),iCr(0,0);
	cout << "CREO LE 3 MATRICI: Y Cb & Cr\n";
	DimezzaCbCr(l,h,ycbcr,iY,iCb,iCr);
	cout << "SALVO LE 3 IMMAGINI\n";
	iY.writePGM(yy);
	iCb.writePGM(cbcb);
	iCr.writePGM(crcr);
	cout << "CREO L'IMMAGINE PGM RISULTANTE\n";
	makePGM(iY,iCb,iCr,out);
	return EXIT_SUCCESS;
}
