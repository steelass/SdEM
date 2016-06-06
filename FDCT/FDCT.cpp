#include <cstdint>
#include <vector>
#include <fstream>
#include <iostream>
#include <cmath>

#define PI 3.14159265

using namespace std;
/*class cosens {
	size_t _x;
	size_t _y;
	size_t _u;
	size_t _v;
	double _val;
public:
	cosens(size_t x, size_t u, size_t y, size_t v) : _x(x), _u(u) , _y(y) , _v(v){
		_val = (std::cos(((2*x +1)*u*PI)/16))*(std::cos(((2*y +1)*v*PI)/16));
	}

};

void makeCos(vector<cosens> &cosi) {
	for(size_t v = 0; v < 8; ++v) {
		for(size_t u = 0; u < 8; ++u) {
			for(size_t x = 0; x < 8; ++x) {
				for(size_t y = 0; y < 8; ++y) {
					cosi.push_back(cosens(x,u,y,v);
				}
			}
			
		}	
	}

}*/


int main(int argc, char**argv) {
	cout << "PROVA FDCT\n";
	vector<float> blocco;
	vector<float> t;
	vector<float> cos;
	for(size_t i = 0; i < 64; ++i) {
		blocco.push_back(81);
	}
	for(auto &x : blocco) {
		x-=128;
	}
	double Cu,Cv;
	float Suv;
	for(size_t v = 0; v < 8; ++v) {
		for(size_t u = 0; u < 8; ++u) {
			Suv = 0;
			if(u==0) {
				Cu = 1.0/sqrt(2);
			}
			else {
				Cu = 1;
			}
			if(v==0) {
				Cv = 1.0/sqrt(2);
			}
			else {
				Cv = 1;
			}
			for(size_t y = 0; y < 8; ++y) {
				for(size_t x = 0; x < 8; ++x) {
					//cout << ((2*x +1)*u*PI)/16 << " - ";
					//cout << ((2*y +1)*v*PI)/16 << "\n";
					
					double cosx = std::cos(((2*x +1)*u*PI)/16);
					double cosy = std::cos(((2*y +1)*v*PI)/16);
					//cout << "Coseni: "<< cosx << " " << cosy << "\n";
					Suv += blocco[ y * 8 + x]*cosy * cosx;
				}
			}
			Suv *=(1.0/4)*Cv*Cu;
			t.push_back(Suv);
		}
	}

	return EXIT_SUCCESS;
}
