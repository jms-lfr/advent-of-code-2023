#include <iostream> 
#include <string>
#include <fstream>

int spellCompare(const std::string &, const std::size_t);

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
			}else if( (left = spellCompare(line, i)) ){
				break; 
			}
		}
		for(std::size_t i = line.length(); i != 0; --i){
			if( isdigit(line[i-1]) ){
				right = line[i-1] - '0';
				break;
			}else if( (right = spellCompare(line, i-1)) ){
				break;
			}
		}
		sum += (left * 10) + right; 
	}

	std::cout << "Day 1 Part 2: " << sum << '\n';
	input.close();
	return 0;
}

int spellCompare(const std::string &str, const std::size_t index){
	// magic literals in string.compare() == lengths of the spelled-out digits 
	switch(str[index]){
		case 'o':
			if(str.compare(index, 3, "one") == 0) return 1;
			break;
		case 't':
			if(str.compare(index, 3, "two") == 0) return 2;
			if(str.compare(index, 5, "three") == 0) return 3;
			break;
		case 'f':
			if(str.compare(index, 4, "four") == 0) return 4;
			if(str.compare(index, 4, "five") == 0) return 5;
			break;
		case 's': 
			if(str.compare(index, 3, "six") == 0) return 6;
			if(str.compare(index, 5, "seven") == 0) return 7;
			break;
		case 'e':
			if(str.compare(index, 5, "eight") == 0) return 8;
			break;
		case 'n':
			if(str.compare(index, 4, "nine") == 0) return 9;
			break;
		default:
			return 0;
	}
	return 0;
}
