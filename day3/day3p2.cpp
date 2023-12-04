#include <iostream> 
#include <fstream>
#include <vector>

using std::size_t;

int gearRatio(std::vector<std::string> &, size_t, size_t);
int checkLeft(std::string &, size_t);
int checkRight(std::string &, size_t);
int beginningAndConvert(std::string &, size_t);
void checkUpOrDown(std::string &, size_t, std::vector<int> &);

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
	int sumP2 = 0;
	std::string line;
	std::vector<std::string> lines;
	lines.reserve(1000);
	while(getline(input, line)){
		lines.push_back(line);
	}

	for(std::size_t i = 0; i < lines.size(); ++i){
		std::string line = lines[i];
		for(std::size_t j = 0; j < line.length(); ++j){
			if(line[j] == '*'){
				sumP2 += gearRatio(lines, i, j);
			}
		}
	}
	
	std::cout << "Day 3 Part 2: " << sumP2 << '\n';
	return 0;
}

int gearRatio(std::vector<std::string> &lines, size_t lineNum, size_t index){
	// index = location of '*' in str
	std::vector<int> factors; 
	factors.reserve(6);
	int result;
	
	// this could definitely be way cleaner
	if( (lineNum == 0) ){
		// down/sides only 
		if( (result = checkLeft(lines[lineNum], index)) ){
			factors.push_back(result);
		}
		if( (result = checkRight(lines[lineNum], index)) ){
			factors.push_back(result);
		}
		checkUpOrDown(lines[lineNum+1], index, factors); // check down
	}else if ( lineNum == lines.size() ){
		// up/sides only
		if( (result = checkLeft(lines[lineNum], index)) ){
			factors.push_back(result);
		}
		if( (result = checkRight(lines[lineNum], index)) ){
			factors.push_back(result);
		}
		checkUpOrDown(lines[lineNum-1], index, factors); // check up
	} else {
		// all
		if( (result = checkLeft(lines[lineNum], index)) ){
			factors.push_back(result);
		}
		if( (result = checkRight(lines[lineNum], index)) ){
			factors.push_back(result);
		}
		checkUpOrDown(lines[lineNum-1], index, factors); // check up
		checkUpOrDown(lines[lineNum+1], index, factors); // check down
	}
	
	return factors.size() == 2 ? factors[0] * factors[1] : 0;
}

void checkUpOrDown(std::string &str, size_t index, std::vector<int> &factors){
	if(isdigit(str[index])){
		factors.push_back(beginningAndConvert(str, index));
	}else{
		int result; 
		if( (result = checkLeft(str, index)) ){
			factors.push_back(result);
		}
		if( (result = checkRight(str, index)) ){
			factors.push_back(result);
		}
	}
}

int checkLeft(std::string &str, size_t index){
	if(index > 0 && isdigit(str[index-1])){
		return beginningAndConvert(str, index);
	}
	return 0;
}

int checkRight(std::string &str, size_t index){
	if( (index < str.length()-1) && isdigit(str[index+1])){
		return atoi(str.c_str() + index+1);
	}
	return 0;
}

int beginningAndConvert(std::string &str, size_t index){
	while(index != 0 && isdigit(str[index-1])){
		// if previous index is a digit, go there
		--index;
	}
	return atoi(str.c_str() + index);
}
