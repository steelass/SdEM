#if ! defined IMAGE_H
#define IMAGE_H

#include <cstdint>
#include <vector>
#include <iostream>
#include <fstream>
#include <iterator>
#include <cstring>
#include <string>

template <typename T>
class image{
	std::size_t height, lenght;
	std::vector<T> matrix;

public:
	image() {}
	image(std::size_t h, std::size_t l) : height(h), lenght(l), matrix(h * l) {}
	std::size_t h() const{ return height; }
	std::size_t l() const{ return lenght; }
	std::uint8_t& operator()(size_t x, size_t y) {
		return matrix[ y * l() + x ];
	}
	std::vector<T>& getMatrix() {
		return matrix;
	}

	/*FUNZIONI PER WRITE*/
	char* dataptr() {
		return reinterpret_cast <char*>(&matrix[0]);
	}
	std::size_t datasize() const {
		return l() * h() * sizeof(T);
	}

	/*ITERATORI*/
	auto begin() -> decltype(matrix.begin()) { return matrix.begin(); }
	auto begin() const -> decltype(matrix.begin()) { return matrix.begin(); }
	auto end() -> decltype(matrix.end()) { return matrix.end(); }
	auto end() const -> decltype(matrix.end()) { return matrix.end(); }
};

class imagePGM : public image<std::uint8_t> {
	std::string descr;

public:
	
	imagePGM(std::size_t h, std::size_t l,bool plain = false) : image(h, l), descr("")  {
		
	}
	imagePGM(std::size_t h, std::size_t l,std::string d,bool plain = false) : image(h, l)  {
		/*std::string s(plain ? "P2":"P5");
		s = s + "\n#"+ descr +"\n";
		s = s + std::to_string(l)+ " "+ std::to_string(h) + "\n255";*/	
		descr = d;
	}
	//MOLTO IMPORTANTE COSTRUTTORE SOTTOCLASSI TRAMITE COSTRUTTORE SUPERCLASSE
	//imagePGM(size_t h, size_t l, std::string descr) : image(h, l), descr(descr) {}

	void setDescr(std::string d) {
		descr.clear();
		descr.append(d);
	}

	std::string getDescr() const{
		return descr;
	}


	void setMatrix(std::vector<std::uint8_t> data){
		getMatrix().swap(data);

	}
	/*SCRITTURA PGM:
		- SE PLAIN DEVO SCRIVERE L'INTERO IN ASCII + " "
		- SE NOT PLAIN DEVO SCRIVERE CON UNA WRITE E CAST A PUNTATORE PRIMO INDIRIZZO DI MEMORIA char*
	*/
	void writePGM(std::ofstream& out, bool plain=false) {
		
		out << (plain ? "P2" : "P5") << "\n";
		out << getDescr();
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
			for(const auto& x: getMatrix()) {
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

};

/*LETTURA PGM*/
std::string readPGM(std::ifstream& in,int &l, int &h,std::vector<std::uint8_t> &v, bool plain = true) {
		std::string ret("");
		std::string s;
		//size_t l,h;
		

		getline(in,s);
		if(s.compare("P2") && plain ) {
			std::cout << "Magic number sbagliato\n";
			return ret;
		}
		else{
			if(s.compare("P5") && !plain ) {
				std::cout << "Magic number sbagliato\n";
				return ret;
			}
		}
		//ret.append(s + "\n");
		getline(in,s);
			/*if(s.at(0)!="#"){
				std::cout << "Commento sbagliato";
				return "";
			}*/
		ret.append(s + "\n");
		in >> l;
		//ret.append(to_string(l)+ " ");
		in >> h;
		//ret.append(to_string(h)+ "\n");
		getline(in,s);
		getline(in,s);
		if (s != "255") {
			std::cout << "Terminatore sbagliato\n";
			return "";
		}
		//ret.append(s + "\n");
		int  num;
		if(plain) {
			while(!in.eof()) {
				in >> num;
				v.push_back(num);
			}
		}
		else {
			int pos = in.tellg();
			in.seekg (0, in.end);
			int length = in.tellg();
			length = length - pos;
			in.seekg (pos, 0);
			v.resize(length);
			in.read(reinterpret_cast<char*>(&v[0]),length*sizeof(uint8_t));

		}


		return ret;
	};

#endif