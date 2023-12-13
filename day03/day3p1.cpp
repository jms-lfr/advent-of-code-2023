#include <iostream>
#include <fstream>
#include <vector>

using std::size_t;

bool checkAround(std::vector<std::string> &, size_t, size_t);
int beginningAndConvert(std::string &, size_t);

// these return ints because I planned for part 2 to  
// require knowing what symbol a number was next to
int checkLeft(std::string &, size_t);
int checkRight(std::string &, size_t);

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
	int sumP1 = 0;
	std::string line;
	std::vector<std::string> lines;
	lines.reserve(1000);
	while(getline(input, line)){
		lines.push_back(line);
	}
	input.close();

	for(size_t i = 0; i < lines.size(); ++i){
		std::string line(lines[i]);
		for(size_t j = 0; j < line.length(); ++j){
			if(isdigit(line[j]) && checkAround(lines, i, j)){
				sumP1 += beginningAndConvert(line, j);
			}
		}
	}

	std::cout << "Day 3 Part 1: " << sumP1 << '\n';
}

int beginningAndConvert(std::string &str, size_t index){
	while(index != 0 && isdigit(str[index-1])){
		// if previous index is a digit, go there
		--index;
	}

	int total;
	total = atoi(str.c_str() + index);
	
	while(index < str.size() && isdigit(str[index])){
		// set all digits to . so we don't double count
		str[index] = '.';
		++index;
	}

	return total;
}

bool checkAround(std::vector<std::string> &lines, size_t lineNum, size_t index){
	bool found = false;
	found = checkLeft(lines[lineNum], index) || checkRight(lines[lineNum], index);

	// this could definitely be way cleaner
	if(lineNum == 0){
		// can't check up
		if(lines[lineNum+1][index] != '.' && !isdigit(lines[lineNum+1][index])){
			found = true;
		}
		found = found || checkLeft(lines[lineNum+1], index) || checkRight(lines[lineNum+1], index);

	}else if( lineNum == lines.size()-1 ){
		// can't check down
		if(lines[lineNum-1][index] != '.' && !isdigit(lines[lineNum-1][index])){
			found = true;
		}
		found = found || checkLeft(lines[lineNum-1], index) || checkRight(lines[lineNum-1], index);
	}else{
		// check up
		if(lines[lineNum-1][index] != '.' && !isdigit(lines[lineNum-1][index])){
			found = true;
		}
		found = found || checkLeft(lines[lineNum-1], index) || checkRight(lines[lineNum-1], index);
		// check down
		if(lines[lineNum+1][index] != '.' && !isdigit(lines[lineNum+1][index])){
			found = true;
		}
		found = found || checkLeft(lines[lineNum+1], index) || checkRight(lines[lineNum+1], index);
	}
	return found;
}

int checkLeft(std::string &str, size_t index){
	if(index > 0 && str[index-1] != '.' && !isdigit(str[index-1])){
		return 1;
	}
	return 0;
}

int checkRight(std::string &str, size_t index){
	if( (index < str.length()-1) && str[index+1] != '.' && !isdigit(str[index+1])){
		return 1;
	}
	return 0;
}
