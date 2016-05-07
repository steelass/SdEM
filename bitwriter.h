#if !defined BIT_H
#define BIT_H

#include <ostream>
#include <cstdint>

class bitwriter {
	std::ostream& _out;
	std::uint8_t _buf;
	int _n;

public:
	bitwriter(std::ostream &out, int n=0) : _out(out) , _n(n) {}
	/*quando viene eliminato l'oggetto devo finire di scrivere il buffer se c'� qualcosa dentro*/
	~bitwriter() {
		flush();
		//flushJack();
	}

	

	/*finch� n � maggiore di zero esegue la scrittura nel buffer del bit preso dall'elelemtno spostato di n*/
	void operator()(std::uint32_t el,int n){
		while(n-- > 0){
			write(el >> n);
		}

	}
	/*scrive sul buffer l'ultimo bit, aumenta n e controlla se � da scrivere il buffer nello stream*/
	void write(std::uint32_t bit){
		_buf = (_buf << 1) | (bit & 1);
		_n++;
		if(_n==8){
			_out.put(_buf);
			_n=0;
		}

	}
	/*finch� _n > 0 scrive sul buffer degli zeri
	quando _n arriva a 8 scrive tutto il buffer sullo stream
	e si azzera n*/
	void flush(std::uint32_t bit = 0){
		while (_n)
			write(bit);
	}

	/*finch� _n � maggiore di zero, riempio di 0 il buffer
	che poi verr� stampato*/
	void flushJack(std::uint32_t bit = 0){
		while(_n) {
			writeJack(bit,1);
		}
	}
	/*finch� la dim � maggiore di zero
	scrivo sul buffer il bit pi� significativo dell'elemento
	passato (el).
	Se _n arriva a 8, scrivo il buffer sullo stream e lo azzero
	*/
	void writeJack(std::uint32_t el, int dim){
		while(dim-- >0){
			_buf = (_buf << 1) | ((el >> dim) & 1);
			_n++;
			if (_n==8) {
				_out.put(_buf);
				_n = 0;
			}

		}

	}	
};

/*
NOTE:
Quando uso il bit writer lo devo creare con 3 campi: stream di output, dimensione del buffer da stampare e buffer da stampare.
Quando devo scrivere passo l'elemento da scrivere e la base con cui scriverlo.
Per stampare bit a bit, finch� la base-1 � maggiore di zero, traslo a sinistra il buffer e ci aggiungo il bit pi�
significativo dell'elemento (traslato a destra di dim ed in & con un bit 1). Aggiorno n(dim del buffer) e se arriva a 8, butto il buffer sullo stream
azzerandolo.
Poich� i bit finali devono essere riempiti di 0 se il buffer non � pieno, occorre richiamare una funzione di flush che, finch� _n (dim del buffer)
� maggiore di 0, mette sul buffer un bit a 0. In questo modo quando il buffer arriver� ad avere 8 elementi, sar� stampato nello stream e n settato a 0.

*/

#endif // BIT_H