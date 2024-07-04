#include <string>
#include <iostream>
#include <fstream>

int main() {
	std::ifstream fin("Home/src/example.dat");
	std::string linear;
	while (getline(fin, linear)) {
		if (linear.length() > 0) {
			std::cout << "\"" << linear;
			std::cout << "\"\n";
		}
	}

}