#if ! defined IMAGEPPM_H
#define IMAGEPPM_H

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
	std::uint8_t& operator()(std::size_t x, std::size_t y) {
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
		
		/*string s(plain ? "P2":"P5");
		s = s + "\n#"+ descr +"\n";
		s = s + to_string(l)+ " "+ to_string(h) + "\n255";*/	
		descr = d;
	}
	
	//MOLTO IMPORTANTE COSTRUTTORE SOTTOCLASSI TRAMITE COSTRUTTORE SUPERCLASSE
	//imagePGM(size_t h, size_t l, string descr) : image(h, l), descr(descr) {}

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
				cout << "Commento sbagliato";
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
			size_t pos = in.tellg();
			in.seekg (0, in.end);
			size_t length = in.tellg();
			length = length - pos;
			in.seekg (pos, 0);
			v.resize(length);
			in.read(reinterpret_cast<char*>(&v[0]),length*sizeof(uint8_t));

		}


		return ret;
	};

class RGB {
	std::uint8_t R;
	std::uint8_t G;
	std::uint8_t B;
	
public:
	RGB() {}
	RGB(std::uint8_t r,std::uint8_t g,std::uint8_t b) : R(r), G(g), B(b) {}
	std::uint8_t getR() { return R;}
	std::uint8_t getG() { return G;}
	std::uint8_t getB() { return B;}

	void setRGB(std::uint8_t r, std::uint8_t g, std::uint8_t b) {
		R = r;
		G = g;
		B = b;
	}

};

class imagePPM : public image<RGB> {
	std::string descr;
public:
	
	imagePPM(std::size_t h, std::size_t l,std::string Descr,bool descrOK = true) : image(h, l)  {
		if(!descrOK) {
			std::string s("P6");
			s = s + "\n";
			if (Descr[0]!='#') 	
				s + "#";
			s = s + Descr +"\n";
			s = s + std::to_string(l)+ " "+ std::to_string(h) + "\n255\n";
			descr = s;
		}
		else
			descr = Descr;
	}

	void setMatrix(std::vector<RGB> data){
		getMatrix().swap(data);
	}

	const std::string getDescr() {
		return descr;
	}

	void writePGM(std::ofstream& out) {
		out << getDescr();
		out.write(dataptr(),datasize());
	}

	

};

std::string readPPM(std::ifstream& in,std::size_t &l, std::size_t &h,std::string &descr,std::vector<RGB> &v) {
	std::string ret("");
	std::string s;
	getline(in,s);
	if(s.compare("P6")) {
		std::cout << "Magic number sbagliato\n";
		return ret;
	}
	
	ret.append(s + "\n");
	getline(in,s);
	if(s[0]!= '#'){
		std::cout << "Commento sbagliato";
		return "";
	}
	ret.append(s + "\n");
	descr.assign(s);
	in >> l;
	ret.append(std::to_string(l)+ " ");
	in >> h;
	ret.append(std::to_string(h)+ "\n");
	getline(in,s);
	getline(in,s);
	if (s != "255") {
		std::cout << "Terminatore sbagliato\n";
		return "";
	}
	ret.append(s + "\n");
	
	int pos = in.tellg();
	in.seekg (0, in.end);
	int length = in.tellg();
	length = length - pos;

	in.seekg (pos, 0);
	v.resize(l*h);
	in.read(reinterpret_cast<char*>(&v[0]),length*sizeof(RGB));

	return ret;
}

void DimezzaR(std::size_t &l,std::size_t &h,std::vector<RGB> &in, std::vector<RGB> &out, bool interpolazione = false) {
	bool m = false;
	std::vector<RGB> v;
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

	float a = float(9)/float(16);
	float b = float(3)/float(16);
	float c = float(3)/float(16);
	float d = float(1)/float(16);
	uint8_t R,G,B;
	RGB p(0,0,0);
	if(!interpolazione) {
		//metodo rustico
		/*prendo i il pixel e i 3 intorno e ne faccio la media in R, G , B*/
		for(std::size_t y=0; y < h-1; y+=2) {
			for (std::size_t x = 0; x < l-1; x+=2) {
				R = (v[y*l+x].getR()+  v[y*l+x+1].getR() + v[(y+1)*l+x].getR() + v[(y+1)*l+x+1].getR())/4 ;
				G = (v[y*l+x].getG()+  v[y*l+x+1].getG() + v[(y+1)*l+x].getG() + v[(y+1)*l+x+1].getG())/4 ;
				B = (v[y*l+x].getB()+  v[y*l+x+1].getB() + v[(y+1)*l+x].getB() + v[(y+1)*l+x+1].getB())/4 ;
		
				p.setRGB(R,G,B);
				out.push_back(p);
			}
		}

	}
	else{	
		//interpolazione
		/*faccio l'interpolazione del pixel e dei 3 pixel intorno assegnado dei pesi*/
		for(std::size_t y = 0; y < h; y+=2) {
			for (std::size_t x = 0; x < l; x+=2) {
				if((x==l-1 && y == 0) || (x==l-1 && y ==h-2)) {
					R = a*v[y*l+x].getR() + b*v[y*l+x].getR() + c*v[y*l+x].getR() + d*v[y*l+x].getR();
					G = a*v[y*l+x].getG() + b*v[y*l+x].getG() + c*v[y*l+x].getG() + d*v[y*l+x].getG();
					B = a*v[y*l+x].getB() + b*v[y*l+x].getB() + c*v[y*l+x].getB() + d*v[y*l+x].getB();
				}
				else {
					if(x == l-1 || y == h-1) {

						if(x ==l-1) {
							R = a*v[y*l+x].getR() + b*v[(y+1)*l+x].getR() + c*v[y*l+x].getR() + d*v[(y+1)*l+x].getR();
							G = a*v[y*l+x].getG() + b*v[(y+1)*l+x].getG() + c*v[y*l+x].getG() + d*v[(y+1)*l+x].getG();
							B = a*v[y*l+x].getB() + b*v[(y+1)*l+x].getB() + c*v[y*l+x].getB() + d*v[(y+1)*l+x].getB();
						}else { //y ==0 || y == h-2
							R = a*v[y*l+x].getR() + b*v[y*l+x+1].getR() + c*v[y*l+x].getR() + d*v[y*l+x+1].getR();
							G = a*v[y*l+x].getG() + b*v[y*l+x+1].getG() + c*v[y*l+x].getG() + d*v[y*l+x+1].getG();
							B = a*v[y*l+x].getB() + b*v[y*l+x+1].getB() + c*v[y*l+x].getB() + d*v[y*l+x+1].getB();
						}
					}
					else { //quando sono al centro
						//cout << "y = " << y << ";x= " << x; 
						R = a*(v[y*l+x].getR()) + b*(v[y*l+x+1].getR()) + c*(v[(y+1)*l+x].getR()) + d*(v[(y+1)*l+x+1].getR());
						G = a*v[y*l+x].getG() + b*v[y*l+x+1].getG() + c*v[(y+1)*l+x].getG() + d*v[(y+1)*l+x+1].getG();
						B = a*v[y*l+x].getB() + b*v[y*l+x+1].getB() + c*v[(y+1)*l+x].getB() + d*v[(y+1)*l+x+1].getB();
					}
				} 
				p.setRGB(R,G,B);
				out.push_back(p);
			}//fine for x
		}//fine for y

	}
	l = l/2;
	h = h/2;
	
}
#endif