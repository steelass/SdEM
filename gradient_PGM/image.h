#if ! defined IMAGE_H
#define IMAGE_H

#include <cstdint>
#include <vector>
#include <iostream>
#include <fstream>
#include <iterator>

class image{
	std::size_t height, lenght;
	std::vector<std::uint8_t> matrix;

public:
	image(std::size_t h, std::size_t l) : height(h), lenght(l), matrix(h * l) {}
	std::size_t h() const{ return height; }
	std::size_t l() const{ return lenght; }
	std::uint8_t& operator()(size_t x, size_t y) {
		return matrix[ y * l() + x ];
	}
	
	/*FUNZIONI PER WRITE*/
	char* dataptr() {
		return reinterpret_cast <char*>(&matrix[0]);
	}
	std::size_t datasize() const {
		return l() * h() * sizeof(uint8_t);
	}

	/*ITERATORI*/
	auto begin() -> decltype(matrix.begin()) { return matrix.begin(); }
	auto begin() const -> decltype(matrix.begin()) { return matrix.begin(); }
	auto end() -> decltype(matrix.end()) { return matrix.end(); }
	auto end() const -> decltype(matrix.end()) { return matrix.end(); }
	
	/*SCRITTURA PGM:
		- SE PLAIN DEVO SCRIVERE L'INTERO IN ASCII + " "
		- SE NOT PLAIN DEVO SCRIVERE CON UNA WRITE E CAST A PUNTATORE PRIMO INDIRIZZO DI MEMORIA char*
	*/
	void writePGM(std::ofstream& out, bool plain=false) {
		//out << "P5" << "\n";
		out << (plain ? "P2" : "P5") << "\n";
		out << "#" << "Una descrizione!" << "\n";
		out << l() << " ";
		out << h() << "\n";
		out << "255" << "\n";
		if(plain){
			/*
			//METODO PIU' RUSTICO
			for(size_t i = 0; i < h() ; ++i) {
				for(size_t j = 0; j < l() ; ++j) {
					out << int((j,i)) <<" ";
				}
			}*/
			for(const auto& x: matrix) {
				out << int(x) << " ";
			}

			//USO DI ITERATORI
			/*
			copy(begin(),end(),ostream_iterator<int>(out," "));
			*/

		}
		else{
			/*
			//SCRITTURA PIU' RUSTICA
			for(size_t i = 0; i < h() ; ++i) {
				for(size_t j = 0; j < l() ; ++j) {
					out.write(reinterpret_cast<const char*>(&(j,i)),1);
				}
			}
			*/
			out.write(dataptr(),datasize());
			
		}
	}

}
#endif