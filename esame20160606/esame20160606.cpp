#include <cstdint>
#include <vector>
#include <fstream>
#include <iostream>
#include <string>
#include "bitreader.h";

using namespace std;
class bitreader {
	ifstream &in;
	uint8_t buf;
	uint8_t n = 0;

	uint8_t read() {
		if (n == 0) {
			buf = in.get();
			n = 8;
		}
		n--;
		return (buf >> n) & 1;
	}

public:
	bitreader(ifstream &i) : in(i) {};
	uint64_t read(size_t dim) {
		uint64_t ret = 0;
		while (dim-- > 0) {
			ret = ret << 1 | read();
		}
		return ret;
	}
	uint64_t operator()(size_t dim) {
		return read(dim);
	}
};

struct RGB{
	uint8_t r;
	uint8_t g;
	uint8_t b;

	RGB(uint8_t R, uint8_t GB) : r(R), g(GB), b(GB) {}

	void setRGB(uint8_t R, uint8_t GB) {
		r = R;
		g = GB;
		b = GB;
	}
};

struct VLC {
	RGB sym;
	uint8_t len;
	int cod;

	VLC(RGB s, uint8_t l, uint8_t c = 0) : sym(s), len(l), cod(c) {}

	void setVLC(RGB s, uint8_t l, uint8_t c = 0) {
		sym = s;
		len = l;
		cod = c;
	}

	void setCode(int c) {
		cod = c;
	}
};

void createHuff(bitreader & br, vector<VLC> &tabH) {
	int code = 0;
	int len = 0;
	for (size_t i = 0; i < tabH.size(); ++i) {
		while (len < tabH[i].len) {
			++len;
			code = code << 1 | 0;
		}
		tabH[i].setCode(code);
		++code;
	}
}

bool checkHuff(bitreader & br, vector<VLC> &tabH, RGB &ret) {
	int code = 0;
	int len = 0;
	bool trovato = false;
	for (size_t i = 0; i < tabH.size() && !trovato; ++i) {
		while (len < tabH[i].len) {
			++len;
			code = code << 1 | br(1);
		}
		if (code == tabH[i].cod) {
			ret.setRGB(tabH[i].sym.r, tabH[i].sym.b);
			//ret.setVLC(tabH[i].sym, tabH[i].len, tabH[i].cod);
			trovato = true;
		}
	}
	return trovato;
}

void writePPM(uint32_t l, uint32_t h, size_t index, vector<RGB> &data, string &p) {
	cout << "Scrittura PPM numero " << index << "\n";
	string nome(p);
	int num = index / 10;
	if (index < 1000) {
		if (index < 100) {
			nome.push_back('0');
			if (index < 10) {
				nome.push_back('0');
			}
		}
	}

	nome.append(to_string(index));
	nome.append(".ppm");
	ofstream out(nome, ios::binary);
	if (!out) {
		cout << "Errore apertura stream di output\n";
	}
	out << "P6" << "\n";
	out << l << "\n";
	out << h << "\n";
	out << 255 << "\n";
	out.write(reinterpret_cast<char*>(&data[0]), l*h * sizeof(RGB));
	

	

}

void readImg(size_t index,uint32_t larg, uint32_t h, ifstream &in, vector<VLC> &tabH, bitreader &br, string &p) {
	auto inizio = index * h * larg;
	auto fine = inizio + h * larg;
	vector<RGB> data;
	RGB pixel(0, 0);
	for (size_t i = inizio; i < fine; ++i) {
		checkHuff(br, tabH, pixel);
		data.push_back(pixel);
	}
	writePPM(larg, h, index, data, p);


}

bool readOptimg(ifstream &in, bitreader & br, vector<VLC> tabH, string &prefix) {
	cout << "Lettura magic number\n";
	in.unsetf(ios_base::skipws);
	char c;
	string s;
	for (size_t i = 0; i < 6; ++i) {
		//in.get(c);
		c = br(8);
		s.push_back(c);
	}
	in.setf(ios_base::skipws);
	if (s.compare("OCTIMG")) {
		cout << "Magic Number errato\n";
		return false;
	}
	cout << "Lettura larghezza e altezza\n";
	uint8_t l1,l2,l3,l4,h1,h2,h3,h4;
	uint32_t larghezza, altezza;
	l1 = br(8);
	l2 = br(8);
	l3 = br(8);
	l4 = br(8);
	//in >> l1;
	//in >> l2;
	//in >> l3;
	//in >> l4;
	larghezza = l1 << 24 | l2 << 16 | l3 << 8 | l4;
	//in >> h1;
	//in >> h2;
	//in >> h3;
	//in >> h4;
	h1 = br(8);
	h2 = br(8);
	h3 = br(8);
	h4 = br(8);
	altezza = h1 << 24 | h2 << 16 | h3 << 8 | h4;
	
	cout << "Lettura numImages e numElement\n";
	uint16_t numImages, numElement;
	numImages = br(10);
	uint8_t ne1, ne2;
	//in >> ne1;
	//in >> ne2;
	ne1 = br(8);
	ne2 = br(8);
	numElement = ne1 << 8 | ne2;
	
	cout << "Lettura tabella di Huffman\n";
	uint8_t r, gb, len;
	RGB el(0, 0);
	VLC elTab(el, 0);
	for (size_t i = 0; i < numElement; ++i) {
		r = br(8);
		gb = br(8);
		len = br(5);
		
		el.setRGB(r, gb);
		elTab.setVLC(el, len);
		tabH.push_back(elTab);
	}
	cout << "Creazione codici di Huffman\n";
	createHuff(br, tabH);
	size_t i = 0;
	cout << "Lettura dati\n";
	while (i < numImages) {
		cout << "Lettura immagine " << i << "\n";
		readImg(i, larghezza, altezza, in,tabH, br, prefix);
		++i;
	}

	return true;
}

int main(int argc, char ** argv) {
	cout << "Esame Giugno 2016\n";
	if (argc != 3) {
		cout << "Errore numero di parametri\n "<< argc << "!!!!\n";
		getchar();
		getchar();
		return EXIT_FAILURE;
	}
	string inn(argv[1]);
	if (!inn.find("l2.octimg")) {
		cout << "Errore estensione primo parametro\n";
		getchar();
		getchar();
		return EXIT_FAILURE;
	}
	string prefix(argv[2]);
	ifstream in(inn, ios::binary);
	if (!in) {
		cout << "errore apertura stream di input\n";
		getchar();
		getchar();
		return EXIT_FAILURE;
	}
	cout << "lettura immagine .optimg di input\n";
	bitreader br(in);
	vector<VLC> tabH;
	if (!readOptimg(in,br, tabH,prefix)) {
		cout << "Errore apertura immagine di input\n";
		getchar();
		getchar();
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}