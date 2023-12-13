#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>

using std::size_t;

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
	int sumP2 = 0;
	std::string line;
	std::vector<int> cardCopies; 
	cardCopies.resize(250, 1); // give space for last cards to increment cards that don't exist
	auto cardNum = 0;
	while(getline(input, line)){
		++cardNum;
		std::vector<int> winningNumbers;
		winningNumbers.reserve(10);
		std::stringstream ss(line);
		std::string temp;
		ss >> temp; // remove 'Card'
		ss >> temp; // remove '#:'
		for(auto i = 0; i < 10; ++i){
			ss >> temp;
			winningNumbers.push_back(stoi(temp));
		}
		ss >> temp; // remove '|'

		auto tempSum = 0;
		auto winNum = 0;
		while(ss >> temp){
			if(std::find(winningNumbers.begin(), winningNumbers.end(), stoi(temp)) != winningNumbers.end()){
				if(winNum){
					tempSum *= 2;
				}else{
					tempSum = 1;
				}
				winNum += 1;
				cardCopies[cardNum+winNum] += cardCopies[cardNum];
			}
		}
		sumP2 += cardCopies[cardNum];
		sumP1 += tempSum;
	}
	input.close();
	
	std::cout << "Day 4 Part 1: " << sumP1 << '\n';
	std::cout << "Day 4 Part 2: " << sumP2 << '\n';
	return 0;
}
