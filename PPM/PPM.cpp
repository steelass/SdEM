/*
Si scriva un programma a linea di comando, che accetti come primo parametro il nome di un file in formato PPM e come secondo parametro il nome di un file di output.
Il programma deve leggere l段mmagine in ingresso e salvarla in output con larghezza e altezza dimezzate. 
Se l段mmagine ha larghezza o altezza dispari, replicare l置ltima colonna o riga (o entrambe).
Utilizzando poi i dati ridotti, ricostruire l段mmagine originale raddoppiandone le dimensioni.
Per la riduzione utilizzate la media di ogni blocco 2ﾗ2. 
Per la ricostruzione provate prima da ogni blocco a fare un blocchetto 2ﾗ2 uguale al pixel che state raddoppiando.*/


#include <cstdint>
#include <vector>
#include <iostream>
#include <fstream>
#include <iterator>
#include <cstring>
#include <string>

using namespace std;

template <typename T>
class image{
	size_t height, lenght;
	vector<T> matrix;

public:
	image() {}
	image(size_t h, size_t l) : height(h), lenght(l), matrix(h * l) {}
	size_t h() const{ return height; }
	size_t l() const{ return lenght; }
	uint8_t& operator()(size_t x, size_t y) {
		return matrix[ y * l() + x ];
	}
	vector<T>& getMatrix() {
		return matrix;
	}

	/*FUNZIONI PER WRITE*/
	char* dataptr() {
		return reinterpret_cast <char*>(&matrix[0]);
	}
	size_t datasize() const {
		return l() * h() * sizeof(T);
	}

	/*ITERATORI*/
	auto begin() -> decltype(matrix.begin()) { return matrix.begin(); }
	auto begin() const -> decltype(matrix.begin()) { return matrix.begin(); }
	auto end() -> decltype(matrix.end()) { return matrix.end(); }
	auto end() const -> decltype(matrix.end()) { return matrix.end(); }
};

class imagePGM : public image<uint8_t> {
	string descr;

public:
	
	imagePGM(size_t h, size_t l,bool plain = false) : image(h, l), descr("")  {
		
	}
	imagePGM(size_t h, size_t l,string d,bool plain = false) : image(h, l)  {
		
		/*string s(plain ? "P2":"P5");
		s = s + "\n#"+ descr +"\n";
		s = s + to_string(l)+ " "+ to_string(h) + "\n255";*/	
		descr = d;
	}
	
	//MOLTO IMPORTANTE COSTRUTTORE SOTTOCLASSI TRAMITE COSTRUTTORE SUPERCLASSE
	//imagePGM(size_t h, size_t l, string descr) : image(h, l), descr(descr) {}

	void setDescr(string d) {
		descr.clear();
		descr.append(d);
	}

	string getDescr() const{
		return descr;
	}


	void setMatrix(vector<uint8_t> data){
		getMatrix().swap(data);

	}
	/*SCRITTURA PGM:
		- SE PLAIN DEVO SCRIVERE L'INTERO IN ASCII + " "
		- SE NOT PLAIN DEVO SCRIVERE CON UNA WRITE E CAST A PUNTATORE PRIMO INDIRIZZO DI MEMORIA char*
	*/
	void writePGM(ofstream& out, bool plain=false) {
		
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
string readPGM(ifstream& in,int &l, int &h,vector<uint8_t> &v, bool plain = true) {
		string ret("");
		string s;
		//size_t l,h;
		

		getline(in,s);
		if(s.compare("P2") && plain ) {
			cout << "Magic number sbagliato\n";
			return ret;
		}
		else{
			if(s.compare("P5") && !plain ) {
				cout << "Magic number sbagliato\n";
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
			cout << "Terminatore sbagliato\n";
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
	uint8_t R;
	uint8_t G;
	uint8_t B;
	
public:
	RGB() {}
	RGB(uint8_t r,uint8_t g,uint8_t b) : R(r), G(g), B(b) {}
	uint8_t getR() { return R;}
	uint8_t getG() { return G;}
	uint8_t getB() { return B;}

	void setRGB(uint8_t r, uint8_t g, uint8_t b) {
		R = r;
		G = g;
		B = b;
	}

};

class imagePPM : public image<RGB> {
	string descr;
public:
	
	imagePPM(size_t h, size_t l,string Descr,bool descrOK = true) : image(h, l)  {
		if(!descrOK) {
			string s("P6");
			s = s + "\n#"+ Descr +"\n";
			s = s + to_string(l)+ " "+ to_string(h) + "\n255\n";
			descr = s;
		}
		else
			descr = Descr;
	}

	void setMatrix(vector<RGB> data){
		getMatrix().swap(data);
	}

	const string getDescr() {
		return descr;
	}

	void writePGM(ofstream& out) {
		out << getDescr();
		out.write(dataptr(),datasize());
	}

	

};

string readPPM(ifstream& in,size_t &l, size_t &h,vector<RGB> &v) {
	string ret("");
	string s;
	getline(in,s);
	if(s.compare("P6")) {
		cout << "Magic number sbagliato\n";
		return ret;
	}
	
	ret.append(s + "\n");
	getline(in,s);
	if(s[0]!= '#'){
		cout << "Commento sbagliato";
		return "";
	}
	ret.append(s + "\n");
	in >> l;
	ret.append(to_string(l)+ " ");
	in >> h;
	ret.append(to_string(h)+ "\n");
	getline(in,s);
	getline(in,s);
	if (s != "255") {
		cout << "Terminatore sbagliato\n";
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

void DimezzaR(size_t &l,size_t &h,vector<RGB> &in, vector<RGB> &out, bool interpolazione = false) {
	bool m = false;
	vector<RGB> v;
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
		cout << "Aggiungo colonna\n";
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
		for(size_t y=0; y < h-1; y+=2) {
			for (size_t x = 0; x < l-1; x+=2) {
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
		for(size_t y = 0; y < h; y+=2) {
			for (size_t x = 0; x < l; x+=2) {
				if(/*(x == 0 && y == 0) ||(x==0 && y == h-1) || */(x==l-1 && y == 0) || (x==l-1 && y ==h-2)) {
					R = a*v[y*l+x].getR() + b*v[y*l+x].getR() + c*v[y*l+x].getR() + d*v[y*l+x].getR();
					G = a*v[y*l+x].getG() + b*v[y*l+x].getG() + c*v[y*l+x].getG() + d*v[y*l+x].getG();
					B = a*v[y*l+x].getB() + b*v[y*l+x].getB() + c*v[y*l+x].getB() + d*v[y*l+x].getB();
				}
				else {
					if(/*x == 0 || y == 0 || */x == l-1 || y == h-1) {

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


int main(int argc,char **argv) {

	cout << "APERTURA STREAM INPUT - OUTPUT\n";
	ofstream out("rana_rustic.ppm", ios::binary);
	ofstream out2("rana_inter.ppm", ios::binary);
	ifstream in("rana.ppm", ios::binary);
	if(!in || ! out || !out2) {
		cout << "Errore apertura stream\n";
		return EXIT_FAILURE;
	}

	cout << "LETTURA PPM\n";
	size_t l,h;
	vector<RGB> v;
	vector<RGB> mezzo;
	string descr(readPPM(in,l,h,v));

	cout << "DIMEZZAMENTO RUSTICO\n";
	DimezzaR(l,h,v,mezzo);
	imagePPM i(h,l,"Rana Dimezzata con pastorizia",false);
	i.setMatrix(mezzo);
	i.writePGM(out);

	cout << "INTERPOLAZIONE\n";
	mezzo.clear();
	l = 375;
	h = 266;
	DimezzaR(l,h,v,mezzo,true);
	imagePPM ii(h,l,"Rana Dimezzata con interpolazione",false);
	ii.setMatrix(mezzo);
	cout << "SCRITTURA PPM\n";
	ii.writePGM(out2);

	return EXIT_SUCCESS;
}


