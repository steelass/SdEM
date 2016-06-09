#include <cstdint>
#include <vector>
#include <iostream>
#include <fstream>
#include <string>


using namespace std;

class RGB {
	uint8_t R;
	uint8_t G;
	uint8_t B;

public:
	RGB() : R(0), G(0), B(0) {}
	RGB(uint8_t r, uint8_t g, uint8_t b) : R(r), G(g), B(b) {}
	
	uint8_t r() {
		return R;
	}
	uint8_t g() {
		return G;
	}
	uint8_t b() {
		return B;
	}
	void setR(uint8_t r) {
		R = r;
	}
	void setG(uint8_t g) {
		G = g;
	}
	void setB(uint8_t b) {
		B = b;
	}




};


template < typename T>
class image {
	size_t max_g;
	size_t h;
	size_t l;
	vector<T> data;
		
public:
	image(size_t max, size_t H, size_t L) : max_g(max), h(H), l(L), data(H*L){}
	
	const size_t MaxG() {
		return max_g;
	}

	const size_t L() {
		return l;
	}
	const size_t H() {
		return h;
	}


	
	void setMax(size_t m)  {
		max_g = m;
	}
	void setL(size_t L) {
		l = L;
	}
	void setH(size_t H) {
		h = H;
	}

	void setData(vector<T> d) {
		data.swap(d);
	}

	vector<T> getData() {
		return data;
	}



	T& operator()(size_t x, size_t y) {
		return data[y * l + x];
	}

	const T& operator()(size_t x, size_t y) const {
		return data[y * l + x];
	}
	char* dataptr() {
		return reinterpret_cast<char*>(&data[0]);
	}

	size_t datasize(){
		return l * h *sizeof(T);
	}

	void write(ofstream &out, bool PGM = true){
		out << (PGM ? "P5\n" : "P6\n");
		out << l << " ";
		out << h << " ";
		out << MaxG() << "\n";
		out.write(dataptr(),datasize());
	}


};

uint16_t make16(char c1,char c2) {
	uint16_t el = 0;
	size_t h = 8;
	while(h-->0) {
		el = (el << 1) | (c1 >> h) & 1;
	}
	h = 8;
	while(h-->0) {
		el = (el << 1) | (c2 >> h) & 1;
	}

	return el;
}

image<uint8_t> lp(image<uint8_t> i) {
	size_t w =i.L();
	size_t h = i.H();
	image<uint8_t> out(255, h,w);
	image<uint8_t> out2(255, h,w);

	for(size_t r = 0; r < h; ++r) {
		out(0,r) = i(0,r);
		for (size_t c = 1; c < w - 1; ++c) {
			out(c,r) = i(c-1,r)/4 + i(c,r)/2 + i(c+1,r)/4;
		}
		out(w-1,r) = i(w-1,r);
	}


	for(size_t i = 0;i < w; ++i) {
		out2(i,0) = out(i,0);
	}
	for(size_t r = 1;r < h-1; ++r) {
		for(size_t c = 0 ; c < w; ++c) {
			out2(c,r) = out(c,r-1)/4 + out(c,r)/2 + out(c,r+1)/4;
		}
	}
		
	for (size_t c = 0; c < w; ++c) {
		out2(c, h - 1) = out(c, h - 1);
	}

	return out;

}



uint16_t make16(uint8_t c1,uint8_t c2) {
	uint16_t el = 0;
	size_t h = 8;
	while(h-->0) {
		el = (el << 1) | (c1 >> h) & 1;
	}
	h = 8;
	while(h-->0) {
		el = (el << 1) | (c2 >> h) & 1;
	}

	return el;
}


bool readPGM(ifstream &in,vector<uint16_t> &v,image<uint8_t> &img) {
	string s; 
	int l,h,maxval;
	getline(in,s);
	if(s.compare("P5")){
		cout << "Magic number sbagliato\n";
		return false;
	}
	cout << s << "\n";
	in >> l;
	in >> h;
	cout <<"lunghezza: "<< l << "\naltezza: " << h << "\n";
	in >> maxval;
	if(maxval > 65536) {
		cout << "MaxVal troppo alto\n";
		return false;
	}

	cout << "val max grigi: " << maxval;
	getline(in,s);
	cout << "\n" << s;
	img.setMax(maxval);
	img.setL(l);
	img.setH(h);
	char c1,c2;
	uint8_t cc,ccc;
	uint16_t el;
	while(!in.eof()) {

		in.get(c2);
		in.get(c1);
		//in >> ccc;
		//in >> cc;
		if(in.eof()) {
			break;
		}
		//el = make16(cc,ccc);
		el = make16(c1,c2);
		v.push_back(el);
	}
	
	return true;
	
}

void to8bit(vector<uint16_t> &in, vector<uint8_t>& out) {
	for(auto &x : in) {
		x-=65280;
		out.push_back(x);
	}

}

void popolaRGB(vector<uint8_t> &in, vector<RGB> &out,size_t h, size_t l) {

	size_t x;
	bool r = true;
	for(size_t y = 0; y < h; ++y) {
		r = ((y%2==0) ? true:false);
		for(x = 0; x < l; ++x) {
			if(r){
				if(x%2==0) {
					out[y*l + x].setR(in[y*l+x]);
				}
				else{
					out[y*l + x].setG(in[y*l+x]);
				}
			}
			else {
				if(x%2==0) {
					out[y*l + x].setG(in[y*l+x]);
				}
				else{
					out[y*l + x].setB(in[y*l+x]);
				}
			}


		}
		//r = (r ? false:true);
	}
}


void interpolazioneV(vector<uint8_t> &v,size_t l, size_t h,vector<RGB> &out){
	uint16_t G5,G4,G2,G6,G8;
	uint16_t X1,X3,X7,X9,X5;
	uint16_t AH, AV;
	size_t x;
	for(size_t y = 0; y < h; ++y) {
		for(x = ((y%2==0) ? 0:1 ); x < l; x+=2) {
			X5 = v[y * l +x];
			
			X1 = ((y<2) ? v[ ( y + 2 ) * l + x ] : v[ ( y - 2 ) * l + x ]);
			G2 = ((y<1) ? v[ ( y + 1 ) * l + x ] : v[ ( y - 1 ) * l + x ]);
			G8 = ((y>h-2) ? v[ ( y - 1 ) * l + x ] : v[ ( y + 1 ) * l + x ]);
			X9 = ((y>h-3) ? v[ ( y - 2 ) * l + x ] : v[ ( y + 2 ) * l + x ]);

			X3 = ((x<2) ? v[ y * l + x + 2 ] : v[ y * l + x - 2 ]);
			G4 = ((x<1) ? v[ y * l + x + 1 ] : v[ y * l + x - 1 ]);
			G6 = ((x>l-2) ? v[ y * l + x - 1 ] : v[ y * l + x + 1 ]);
			X7 = ((x>l-3) ? v[ y * l + x - 2 ] : v[ y * l + x + 2 ]);
			 
		
		/*	//centro

			G4 = v[ y * l + x - 1 ];
			G2 = v[ ( y - 1 ) * l + x ];
			G6 = v[ y * l + x + 1 ];
			G8 = v[ ( y + 1 ) * l + x ];
			X1 = v[ ( y - 2 ) * l + x ];
			X3 = v[ y * l + x - 2 ];
			X7 = v[ y * l + x + 2 ];
			X9 = v[ ( y + 2 ) * l + x ];*/

			AH = abs(G4 - G6) + abs(X5 -X3 + X5 -X7);
			AV = abs(G2 - G8) + abs(X5 - X1 + X5 - X9);

			if(AH<AV) {
				G5 = (G4 + G6)/2 + (X5 -X3 + X5 -X7)/4;
			}
			else {
				if(AH > AV) {
					G5 = (G2 + G8)/2 + (X5 - X1 + X5 - X9)/4;
				}
				else {//AH = AV
					G5 = (G4 + G6 + G2 + G8)/4 + (X5 - X1 + X5 - X3 + X5 - X7 + X5 - X9)/8;
				}
			}
			/*if(x%2==1){
				out[y*l+x].setR(X5);
			}
			else {
				out[y*l+x].setB(X5);
			}*/
			/*if(G5 > 65534 || G5 < 0){
				cout << "e: " << G5 << " ";
				G5 = 0;
			}
			else if (G5>255) {
				G5 = 255;
			}*/

			out[y*l+x].setG((G5<0) ? 0:G5);
		}

	}

}

void interpolazione2(vector<uint8_t> &v,size_t l, size_t h,vector<RGB> &out){
	uint16_t G1,G3,G7,G9,G5,G4,G2,G6,G8;
	uint16_t X1,X3,X7,X9,X5;
	uint16_t AN, AP;
	size_t xi;
	uint16_t r,b;
	cout << "Prima parte\n";
	size_t y;
	for(y = 0; y < h; ++y) {
		for(xi = ( (y%2==0) ? 1:0 ); xi < l; xi+=2) {
			r = ((xi==0) ? v[y*l+xi+1] : ((xi==l-1) ? v[y*l+xi-1] : (v[y*l+xi-1] + v[y*l+xi+1])/2));
			b = ((y==0) ? v[(y+1)*l+xi] : ((y==h-1) ? v[(y-1)*l+xi] : (v[(y-1)*l+xi] + v[(y+1)*l+xi])/2));

			
			/*if(r>65534) {
				r = 0;
			}
			if(b>65534) {
				b = 0;
			}
			if(r > 255) {
				r = 255;
			}
			if( b > 255) {
				b = 255;
			}*/

			out[y*l+xi].setR((r<0) ? 0:r);
			out[y*l+xi].setB((b<0) ? 0:b);
		}
	}

	cout << "Seconda parte\n";
	size_t x;
	bool red = true;
	for(y = 0; y < h; ++y) {
		red = ((y%2==0) ? true: false);
		for(x = ( (y%2==0) ? 0:1 ); x < l; x+=2) {
			G5 = out[y * l +x].g();
			

			///*
			X1 =((x<1 && y<1) ? v[(y+1)*l+x+1] : ((x<1) ? v[(y-1)*l+x+1] : ((y<1) ? v[(y+1)*l+x-1] : v[(y-1)*l+x-1])));
			G1 =((x<1 && y<1) ? out[(y+1)*l+x+1].g() : ((x<1) ? out[(y-1)*l+x+1].g() : ((y<1) ? out[(y+1)*l+x-1].g() : out[(y-1)*l+x-1].g())));
			G2 = ((y<1) ? out[ ( y + 1 ) * l + x ].g() : out[ ( y - 1 ) * l + x ].g());

			X3 = ((x>l-2 && y==0) ? v[(y+1)*l+x-1] : ((y<1) ? v[(y+1)*l+x+1] : ((x>l-2) ? v[(y-1)+x-1] : v[(y-1)*l+x+1])));
			G3 = ((x>l-2 && y==0) ? out[(y+1)*l+x-1].g() : ((y<1) ? out[(y+1)*l+x+1].g() : ((x>l-2) ? out[(y-1)+x-1].g() : out[(y-1)*l+x+1].g())));
		
			G4 = ((x<1) ? out[ y * l + x + 1 ].g() : out[ y * l + x - 1 ].g());
			G6 = ((x>l-2) ? out[ y * l + x - 1 ].g() : out[ y * l + x + 1 ].g());
			X7 = ((x>l-2 && y>h-2) ? v[(y-1)*l+x+1] : ((y>h-2) ? v[(y-1)*l+x-1] : ((x<1 && y<1) ? v[(y+1)*l+x+1] : v[(y+1)*l+x-1])));
			G7 = ((x==0 && y>h-2) ? out[(y-1)*l+x+1].g() : ((y>h-2) ? out[(y-1)*l+x-1].g() : ((x<1 && y<1) ? out[(y+1)*l+x+1].g() : out[(y+1)*l+x-1].g())));
			G8 = ((y>h-2) ? out[ ( y - 1 ) * l + x ].g() : out[ ( y + 1 ) * l + x ].g());
			X9 = ((x>l-2 && y>h-2) ? v[(y-1)*l+x-1] : ((y>h-2) ? v[(y-1)*l+x+1] : ((x>l-2) ? v[(y+1)*l+x-1] : v[(y+1)*l+x+1])));
			G9 = ((x>l-2 && y>h-2) ? out[(y-1)*l+x-1].g() : ((y>h-2) ? out[(y-1)*l+x+1].g() : ((x>l-2) ? out[(y+1)*l+x-1].g() : out[(y+1)*l+x+1].g())));
			
			//*/
			/*
			X1 = ((y<2) ? v[ ( y + 2 ) * l + x ] : v[ ( y - 2 ) * l + x ]);
			G1 = ((y<2) ? out[ ( y + 2 ) * l + x ].g() : out[ ( y - 2 ) * l + x ].g());
			G2 = ((y<1) ? out[ ( y + 1 ) * l + x ].g() : out[ ( y - 1 ) * l + x ].g());
			G8 = ((y>h-2) ? out[ ( y - 1 ) * l + x ].g() : out[ ( y + 1 ) * l + x ].g());
			X9 = ((y>h-3) ? v[ ( y - 2 ) * l + x ] : v[ ( y + 2 ) * l + x ]);
			G9 = ((y>h-3) ? out[ ( y - 2 ) * l + x ].g() : out[ ( y + 2 ) * l + x ].g());

			X3 = ((x<2) ? v[ y * l + x + 2 ] : v[ y * l + x - 2 ]);
			G3 = ((x<2) ? out[ y * l + x + 2 ].g() : out[ y * l + x - 2 ].g());
			G4 = ((x<1) ? out[ y * l + x + 1 ].g() : out[ y * l + x - 1 ].g());
			G6 = ((x>l-2) ? out[ y * l + x - 1 ].g() : out[ y * l + x + 1 ].g());
			X7 = ((x>l-3) ? v[ y * l + x - 2 ] : v[ y * l + x + 2 ]);
			G7 = ((x>l-3) ? out[ y * l + x - 2 ].g() : out[ y * l + x + 2 ].g());
			
			//*/
			 
		

			AN = abs(X1 - X9) + abs(G5 - G1 + G5 - G9);
			AP = abs(X3 - X7) + abs(G5 - G3 + G5 - G7);

			if(AN<AP) {
				X5 = (X1 + X9)/2 + (G5 - G1 + G5 - G9)/4;
			}
			else {
				if(AN > AP) {
					X5 = (X3 + X7)/2 + (G5 - G3 + G5 - G7)/4;
				}
				else {//AN = AP
					X5 = (X1 + X3 + X7 + X9)/4 + (G5 - G1 + G5 - G3 + G5 - G7 + G5 - G9)/8;
				}
			}
			
			/*if ( X5 > 65534) {
				X5 = 0;
			}
			if (X5 > 255) {
				X5 = 255;
			}*/


			if ( red) {
				out[y*l+x].setB((X5<0) ? 0:X5);
			}
			else{	
				out[y*l+x].setR((X5<0) ? 0:X5);
			}

		}

	}
}


int main(int argc,char **argv) {
	
	if(argc!=3) {
		cout << "Numero parametri errato\n";
		return 13;
	}
	
	ifstream in(argv[1],ios::binary);
	string out_s(argv[2]);
	out_s.append(".pgm");
	string out_s2(argv[2]);
	out_s2.append(".ppm");
	ofstream out(out_s,ios::binary);
	ofstream out2(out_s2,ios::binary);
	if(!in || !out || !out2) {
		cout << "Errore apertura stream\n";
		return 14;//EXIT_FAILURE;
	}
	vector<uint16_t> v;
	cout << "Lettura PGM\n";
	image<uint8_t> i(0,0,0);
	readPGM(in,v,i);
	cout << "Converto in valori a 8 bit\n";
	vector<uint8_t> vv;
	to8bit(v,vv);
	size_t newMax = i.MaxG();
	newMax /= 256;
	i.setMax(newMax);
	cout << "Scrivo la nuova immagine\n";
	
	i.setData(vv);
	image<uint8_t> lop = lp(i);
	
	lop.write(out);
	size_t l = i.L();
	size_t h = i.H();
	cout << "Inizializzo il vettore RGB\n";
	vector<RGB> rgb;
	size_t c = 0;
	while(c < l * h){
		RGB el(0,0,0);
		rgb.push_back(el);
		++c;
	}
	cout << "Popolo il vettore RGB con i valori del pattern\n";
	popolaRGB(lop.getData(),rgb,h,l);
	
	cout << "Interpolazione verde\n";
	interpolazioneV(vv,i.L(),i.H(),rgb);
	cout << "Interpolazione parte 2\n";
	interpolazione2(vv,l,h,rgb);
	cout << "Creazione Immagine\n";
	image<RGB> i2(newMax, h,l);
	i2.setData(rgb);
	i2.write(out2,false);
	return EXIT_SUCCESS;
}

