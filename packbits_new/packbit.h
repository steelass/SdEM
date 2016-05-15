#if !defined PACKBIT_H
# define PACKBIT_H
#include <vector>
#include <cstdint>
#include <fstream>
#include <iostream>

class packbits {
	std::vector<std::uint8_t> buf;
	bool rip;

public:
	packbits() : rip(false), buf(0) {}
	packbits(bool r) : rip(r), buf(0) {}

	void push(std::uint8_t c) {
		buf.push_back(c);
	}

	const std::vector<std::uint8_t>& getBuf() {
		return buf;
	}


	const bool& getRip() {
		return rip;
	}

	void setRip(bool f){
		rip = f;
	}

	std::uint8_t& operator [] (std::size_t pos) {
		return buf[pos];
	}

	std::uint8_t& at(std::size_t pos) {
		return buf[pos];
	}

	std::uint8_t& operator () (std::size_t pos) {
		return buf[pos];
	}

	const std::uint8_t size() {
		return buf.size();
	}

	void clear() {
		buf.clear();
	}

	bool readfile(std::ifstream &in,std::ofstream &out) {
	
	bool ripetizione = false;
	std::uint8_t c;
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
	std::cout << fine;
	out << fine;
	return true;

	}
	void write(std::ofstream& out,bool evo=false) {
		//se sono in ripetizione dimensione e carattere che si ripete
		if(getRip()) {
			out << 256- size() + 1<< at(0); 
			std::cout << 256 - size() + 1<< at(0) << " "; 
			
		}
		//se non sono in ripetizione stampo il buffer meno l'ultimo carattere
		else{
			int meno = 2;
			if(evo) {
				meno = 1;
			}

			std::cout << size()-meno;
			out << size()-meno; /* meno 2 perchè l'ultimo cararttere nel buf non lo stampo e perchè packbits decrementa di 1*/
			for ( auto &x : getBuf()) {
				if ((x != at(size()-1) && !evo) || evo) {
					out << x;
					std::cout << x << " ";
				}

			}
		}
		clear();
	}

	void decompress (std::ifstream& in, std::ofstream& out) {
		
		std::uint8_t l;
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

	bool readfileEvo(std::ifstream &in,std::ofstream &out) {
	std::uint8_t contR = 0;
	std::uint8_t contS = 0;
	bool ripetizione = false;
	std::uint8_t c;
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
	std::cout << fine;
	out << fine;
	return true;

	}//fine readfileEVO
};

# endif