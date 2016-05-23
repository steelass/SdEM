#if ! defined FREQUENZE_H
#define FREQUENZE_H
class frequency {
	std::map <char,int> coppie;
	float Entropy;

	public:
		frequency(std::size_t i): Entropy(0)  {
			char x = 0;
			for (std::size_t i = 0; i < 256; ++i) {
				coppie[x] = 0;
				++x;				
			}	
		}

		void add(char c) {
			coppie[c]++;
		}

		const std::size_t dim() {
			return coppie.size();
		}

		int at(char c) {
			return coppie[c];
		}

		auto begin() -> decltype(coppie.begin()) {return coppie.begin();}
		auto begin() const -> decltype(coppie.begin()) {return coppie.begin();}

		auto end() -> decltype(coppie.end()) {return coppie.end();}
		auto end() const -> decltype(coppie.end()) {return coppie.end();}	
}

#endif