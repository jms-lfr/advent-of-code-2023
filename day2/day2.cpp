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
	//int redCount = 12, greenCount = 13, blueCount = 14;
	int colorCounts[3] = {12, 13, 14}; // given
	int sumP1 = 0;
	int colorCountsP2[3]; // r = 0, g = 1, b = 2
	int sumP2 = 0;

	std::string line;
	while( getline(input, line) ){
		for(int i = 0; i < 3; ++i){ // reset min needed counts
			colorCountsP2[i] = 0;
		}
		bool possible = true;
		std::stringstream ss(line);
		std::string temp;
		ss >> temp; // remove "Game" (unneeded)

		ss >> temp;
		int id = atoi(temp.c_str()); // needed for P1

		while(ss >> temp){
			int count = atoi(temp.c_str());
			
			ss >> temp;
			int index = temp[0] % 3; // r = 0, g = 1, b = 2
			if(count > colorCounts[index]){
				possible = false;
			}
			//if(!possible) break; // don't need to keep going if it's already impossible
			
			if(count > colorCountsP2[index]){
				colorCountsP2[index] = count;
			}
		}
		
		if(possible) sumP1 += id;
		sumP2 += (colorCountsP2[0] * colorCountsP2[1] * colorCountsP2[2]);
	}
	
	std::cout << "Day 2 Part 1: " << sumP1 << '\n';
	std::cout << "Day 2 Part 2: " << sumP2 << '\n';
	return 0;
}
