#include <cmath>
#include <cstdint>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>

#define M_PI 3.14159265358979323846

using namespace std;

vector<int16_t> leggifile(const string& f, size_t& c) {
	ifstream ifs(f, ios::binary);
	if (!ifs)
		exit(1);

	vector<int16_t> out(512);

	c = 0;
	int16_t tmp;
	while (true) {
		if (ifs.eof())
			break;
		++c;
		ifs.read(reinterpret_cast<char*>(&tmp), sizeof(tmp));
		out.push_back(tmp);
	}

	size_t num = (512 - out.size() % 512) + 512;
	for (size_t i = 0; i < num; i++)
		out.push_back(0);

	return out;
}

void scrivifile(const string& f, const vector<int16_t>& v) {
	ofstream ofs(f, ios::binary);
	if (!ofs)
		exit(2);

	for (const auto& x : v)
		ofs.write(reinterpret_cast<const char*>(&x), sizeof(x));
}

struct finestra {
	vector<int32_t> x;

	finestra(size_t i) : x(i) {}
	int32_t& operator[](size_t i) {
		return x[i];
	}
	const int32_t& operator[](size_t i) const {
		return x[i];
	}
};

struct coseni {
	vector<double> data;
	size_t _N;

	coseni(size_t N) {
		double tmp;
		_N = N;
		for (size_t k = 0; k < N; k++) {
			for (size_t n = 0; n < 2 * N; ++n) {
				tmp = cos((M_PI / N)*(n + 0.5 + N / 2)*(k + 0.5));
				data.push_back(tmp);
			}
		}
	}

	double& operator()(size_t k, size_t n) {
		return data[n + k * 2 * _N];
	}
};

struct seni {
	vector<double> data;

	seni(size_t N) {
		double tmp;
		for (size_t n = 0; n < 2 * N; ++n) {
			tmp = sin(M_PI / (2 * N)*(n + 0.5));
			data.push_back(tmp);
		}
	}

	double& operator[](size_t n) {
		return data[n];
	}
};

int main() {
	size_t M;
	size_t N = 512;

	// Leggo
	cout << "Leggo\n";
	auto x = leggifile("prova_mdct.raw", M);
	size_t nF = x.size() / N;

	// Precalcolo seni e coseni
	coseni C(N);
	seni wn(N);

	// Quantizzazione
	size_t quant = 100000;
	/* cout << "Specifica coefficiente di quantizzazione: ";
	cin >> quant; */

	// Trasformo
	cout << "Trasformo\n";
	vector<finestra> windows;
	for (size_t i = 0; i < nF; ++i) {
		windows.push_back(finestra(N));
		for (size_t k = 0; k < N; ++k) {
			double tmp = 0;
			for (size_t n = 0; n < 2 * N; ++n) {
				tmp += x[n + N * i] * wn[n] * C(k, n);
			}
			windows[i][k] = round(tmp / quant);
		}
	}


	// Ricostruisco
	cout << "Ricostruisco\n";
	vector<int16_t> y(2 * N);
	vector<int16_t> sigout(x.size());
	for (size_t i = 0; i < nF; ++i) {
		for (size_t n = 0; n < 2 * N; ++n) {
			double sum = 0;
			for (size_t k = 0; k < N; ++k) {
				sum += windows[i][k] * C(k, n)*quant;
			}
			y[n] = round(2.0 / N*wn[n] * sum);
		}
		if (i != nF - 1)
			for (size_t n = 0; n < 2 * N; ++n) {
				sigout[n + i*N] += y[n];
			}
		else
			for (size_t n = 0; n < N; ++n) {
				sigout[n + i*N] += y[n];
			}
	}

	// Scrivo
	cout << "Scrivo file\n";
	scrivifile("out_mdct.raw", sigout);
}
