#include <cstdint>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>

using namespace std;
class bit_writer{	
	ofstream &out;
	int n;
	uint8_t buf;

public:
	bit_writer(ofstream &o): out(o), n(0), buf(0) {}

	void write(uint32_t el,size_t h){
		while(h-->0) {
			
			n++;
			uint8_t bit =  (el >> h & 1);
			buf =(buf << 1) | bit;
			if(n==8) {
				out.put(buf);
				n = 0;
			}
		}


	}
	~bit_writer() {
		while(n>0) {
			write(0,1);
		}

	}


};

class bit_reader{
	ifstream &in;
	int n;
	uint64_t bit_pos; 
	uint8_t buf;

public:
	bit_reader(ifstream &i) : in(i), n(0), bit_pos(0), buf(0) {}

	uint32_t operator()(size_t h) {
		
		uint32_t ret = 0;
		bit_pos+=h;
		while(h-->0) {
			ret = (ret << 1) | read();
		}

		return ret;
	}


	uint8_t read() {
		if(n==0) {
			buf = in.get();
			n = 8;
		}
		n--;
		uint8_t bit = buf >> n & 1;
		return bit;
	}

	uint32_t readJ(size_t h) {
		uint32_t ret = 0;
		bit_pos+=h;

		while(h-->0) {

			if(n==0) {
				buf = in.get();
				n = 8;
			}
			n--;

			uint8_t bit = (buf >> n) & 1;
			ret = ret << 1 | bit;
		}

		return ret;
		
		
	}
	
};




int main(int argc, char **argv) {
	cout << "PROVA BIT WRITER\n";
	ofstream out("prova.txt", ios::binary);
	uint8_t el = 31;
	bit_writer bw(out);
	bw.write(el,8);
	cout << "PROVA BIT READER\n";
	out.close();
	ifstream in("prova.txt", ios::binary);
	bit_reader br(in);
	//uint8_t ret = br(8);// 
	uint8_t r2 = br.readJ(8);
	//cout << "ritorno: " << int(ret);
	cout << "ritorno jack: " << int(r2);
	getchar();
	getchar();
	return EXIT_SUCCESS;
}
