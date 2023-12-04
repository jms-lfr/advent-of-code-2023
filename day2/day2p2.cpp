#include <iostream> 
#include <fstream>
#include <sstream>

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
	int sumOfPowers = 0;
	int colorCounts[3] = {0, 0, 0};
	std::string line;
	while( getline(input, line) ){
		std::stringstream ss(line);
		std::string temp;
		ss >> temp; // remove "Game"
		ss >> temp; // remove "[id]:"

		while(ss >> temp){
			int count = atoi(temp.c_str());

			ss >> temp;
			int index = temp[0] % 3; // r = 0, g = 1, b = 2
			if(count > colorCounts[index]){
				colorCounts[index] = count;
			}
		}
		sumOfPowers += (colorCounts[0] * colorCounts[1] * colorCounts[2]);
		for(int i = 0; i < 3; ++i){
			colorCounts[i] = 0;
		}
	}
	
	std::cout << "Day 2 Part 2: " << sumOfPowers << '\n';
	return 0;
}
