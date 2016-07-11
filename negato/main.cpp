#include <cstdint>
#include <iostream>

using namespace std;

uint8_t nega(uint8_t numero) {
	
	uint8_t negato = ~numero;
	cout << "Numero originario: " << int(numero) << "\n";
	cout << "Numero negato(complemementa a 1): " << int(negato) << "\n";
	return negato;
}
template <typename T>
bool checkNbit(size_t len, T toCheck, T el) {
	bool bit, bit2;
	bool ret = false;
	while (len-- > 0) {
		bit = (toCheck >> len) & 1;
		bit2 = (el >> len) & 1;
		if (bit == bit2) {
			ret = true;
		}
		else return false;
	}
	return ret;
}

int main(int argc, char**argv) {
	cout << "Prova negato bit a bit\n";
	uint8_t numero = 0;
	uint8_t negato = nega(numero);
	cout << "Prova del controllo solo per n bit\n";
	uint8_t el = 31;
	size_t n = 4;
	if (checkNbit(n, negato, el)) {
		cout << int(el) << " e " << int(negato) << " sono uguali per " << n << " bit\n";
	}
	else {
		cout << int(el) << " e " << int(negato) << " NON sono uguali per " << n << " bit\n";
	}
	getchar();
	getchar();
}

