#include "../include/parsing.hpp"

// client_max_body_size 1B; = 1
// client_max_body_size 1K; = 1000
// client_max_body_size 1M; = 1 000 000
// client_max_body_size 1G; = 1 000 000 000

void parsing::assignConfigFile(){
	for (int i = 0; i < verifLine.size(); i++) {
		cout << "line " << i + 1 << " : " << verifLine[i] << endl;
	}
}