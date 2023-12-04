#include <iostream> 
#include <string>
#include <fstream>

int main(int argc, char* argv[]){
	// setup
	std::string filename;
	if(argc < 2){
		filename = std::string("input.txt");
	}else{
		filename = std::string(argv[1]);
	}
	std::ifstream input(filename);
	if(!input.is_open()){
		std::cerr << "Unable to open file " << filename << '\n';
		return 1;
	}

	// solving the problem
	int sum = 0; 
	std::string line;
	while( getline(input, line) ){
		int left = 0, right = 0;

		for(std::size_t i = 0; i < line.length(); ++i){
			if( isdigit(line[i]) ){
				left = line[i] - '0';
				break;
			}
		}
		for(std::size_t i = line.length(); i != 0; --i){
			if( isdigit(line[i-1]) ){
				right = line[i-1] - '0';
				break;
			}
		}
		sum += (left * 10) + right; 
	}

	std::cout << "Day 1 Part 1: " << sum << '\n';
	input.close();
	return 0;
}
