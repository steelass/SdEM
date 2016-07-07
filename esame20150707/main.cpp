#include "huffman.h"

#include <string>
#include <fstream>
#include <iterator>
#include <iomanip>
#include <iostream>

using namespace std;

class bitreader {
	ifstream &in;
	uint8_t buf;
	uint8_t n = 0;

	uint8_t readbit() {
		if (n == 0) {
			buf = in.get();
			n = 8;
		}
		--n;
		return (buf >> n) & 1;
	}

public:
	bitreader(ifstream &i) : in(i) {}

	uint32_t read(int nn) {
		uint32_t ret = 0;
		while (nn-- > 0) {
			ret = ret << 1 | readbit();
		}
		return ret;
	}

	bool peek() {
		if (in.peek() == EOF)
			return false;
		else
			return true;
	}
};

class bitwriter {
	vector<uint8_t> data;
	uint8_t buf = 0;
	uint8_t n = 0;

	void writebit(uint32_t bit) {
		
		buf = buf << 1 | bit & 1;
		n++;
		if (n == 8) {
			data.push_back(buf);
			n = 0;
		}
	}

public:
	bitwriter(vector<uint8_t> &d) : data(d) {}

	vector<uint8_t> getData() {
		return data;
	}

	void write(uint32_t el,uint8_t len) {
		uint8_t bit;
		while (len-- > 0) {
			writebit(el >> len);
		}
	}
	void check() {
		if (n > 0) {
			while (n > 0) {
				write(1, 1);
			}
		}
	}
};

struct vlc {
	uint8_t sym;
	int cod;
	uint8_t len;

	vlc(uint8_t Sym, int Cod, uint8_t Len) : sym(Sym), cod(Cod), len(Len) {}
};

struct hufstr {
	vector<vlc> table;

	hufstr(const string &filename) {
		ifstream in(filename, ios::binary);
		if (!in){
			cout << "Errore apertura stream\n";
			EXIT_FAILURE;
		}
		in.unsetf(ios_base::skipws);
		uint8_t s,sp,n;
		int l;
		while(true){
			in.peek();
			if (!in) {
				break;
			}

			s = in.get();
			sp =in.get();
			in >> l;
			n = in.get();
			table.push_back(vlc(s, 0, l));
		}
		createHuff();
	}

	void createHuff() {
		uint8_t len = 1;
		int cod = 0;
		for (size_t i = 0; i < table.size(); ++i) {
			while (len < table[i].len) {
				len++;
				cod = cod << 1;
			}
			table[i].cod = cod;
			cod++;
		}
	}

	bool checkHuff(bitreader &br, uint8_t &c, size_t &lun) const{
		uint8_t len = 0;
		int cod = 0;
		bool trovato = false;

		for (size_t i = 0; i < table.size() && !trovato; ++i) {
			while (len < table[i].len) {
				len++;
				lun++;
				cod = cod << 1 | br.read(1);
			}
			if (cod == table[i].cod) {
				trovato = true;
				c = table[i].sym;
			}
		}
		
		return trovato;
	}

	void compress(const string &s, bitwriter &br) const {
		bool trovato;
		for (auto &x : s) {
			cout << "carattere: " << x << "\n";
			trovato = false;
			for (auto &y : table) {
				if (x == y.sym) {
					cout << y.sym << ": " << int(y.cod) << "(lunghezza: " << y.len << ")\n";
					br.write(y.cod,uint8_t(y.len));
					trovato = true;
				}
				if (trovato)
					break;
			}
		}
		br.check();
		
	}

	string decompress(vector<uint8_t> &in) const {
		ofstream out("temp.data", ios::binary);
		out.unsetf(ios_base::skipws);
		if (!out) {

		}
		for (auto &x : in) {
			out.put(x);
		}
		size_t maxL = 8 * in.size();
		out.close();
		ifstream inn("temp.data", ios::binary);
		inn.unsetf(ios_base::skipws);
		if (!inn) {

		}
		uint8_t c = 0;
		
		string ret("");
		size_t lun = 0;
		bitreader br(inn);
		while (true) {
			if (!checkHuff(br, c, lun)) {
				break;
			}
			ret.push_back(c);
			cout << ret << "\n";
			if (lun >= maxL) {
				break;
			}
			
		}
		return ret;
		
	}
};

string to_binary(uint8_t len, uint32_t code) {
	string s;
	while (len > 0) {
		s = (code & 1 ? '1' : '0') + s;
		code >>= 1;
		--len;
	}
	return s;
}

void writeJack(ofstream &out, huffman<uint8_t> &huff){
	for (const auto& x : huff.table()) {
		out.put(x._sym);
		out << " ";
		out << int(x._len);
		out << "\n";
	}
}

int main() {
	cout << "Apro il file e non salto i whitespace\n";
	ifstream is("bibbia.txt", ios::binary);
	is.unsetf(ios_base::skipws);

	cout << "Stimo le frequenze dei byte nel file\n";
	frequency<uint8_t> f;
	f = for_each(istream_iterator<char>(is), istream_iterator<char>(), f);
	// Aggiungo 1 per ogni possibile byte (per essere sicuro di avere 
	// un codice di Huffman per ogni possibile simbolo)
	for (int i = 0; i < 256; ++i)
		f(i);
	cout << "Calcolo i codici canonici di Huffman\n";
	huffman<uint8_t> huff(f);

	cout << "Output di esempio dei codici\n";
	ofstream os("huffman.txt");
	for (const auto& x : huff.table()) {
		if (x._sym >= 32)
			os.put(x._sym);
		else
			os << "\\x" << hex << setfill('0') << setw(2) << uint32_t(x._sym);

		os << " - " << to_binary(x._len, x._code) << "\n";
	}


	cout << "PARTE DI JACK\n";
	cout << "Apertura stream di output\n";
	ofstream out("huffman.jack", ios::binary);
	if (!out){
		cout << "Errore apertura stream di output\n";
		return EXIT_FAILURE;
	}
	cout << "Scrittura tabella di huffman nel formato scelto\n";
	writeJack(out, huff);
	out.close();
	cout << "Lettura della tabella salvata e creazione di un hufstr\n";
	hufstr hstr("huffman.jack");
	string input("Maaaa");
	cout << "Compressione di una stringa: " << input <<"\n";
	vector<uint8_t> v;
	bitwriter br(v);
	hstr.compress(input,br);
	cout << "Decompressione della stessa stringa\n";
	auto s = hstr.decompress(br.getData());
	cout << "Stringa ricostruita: " << s << "\n";
	cout << "ok";
}