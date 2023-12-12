#include <iostream>
#include <fstream>
#include <vector>
#include <sstream> 

using std::size_t;

long long traverse(const std::vector<long long> &, const bool=true);

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

	// solve the problem
	long long resultP1 = 0;
	long long resultP2 = 0;
	std::vector<std::vector<long long>> histories; 
	histories.reserve(200);
	std::string line;
	while(getline(input, line)){
		std::stringstream ss(line);
		std::vector<long long> sequence;
		while(ss >> line){
			sequence.push_back(stoi(line));
		}
		histories.push_back(std::move(sequence));
	}
	input.close();
	

	for(const auto& vec : histories){
		resultP1 += vec[vec.size()-1] + traverse(vec);
		resultP2 += vec[0] - traverse(vec, false);
	}

	std::cout << "Day 9 Part 1: " << resultP1 << '\n';
	std::cout << "Day 9 Part 2: " << resultP2 << '\n';
	return 0;
}

long long traverse(const std::vector<long long> &vec, bool forward){
	if(vec.size() == 0){
		return 0;
	}
	bool allZero = true;
	for(const auto &num : vec){
		if(num != 0){
			allZero = false;
			break;
		}
	}
	if(allZero) return 0;
	std::vector<long long> next;
	for(size_t i = 1; i < vec.size(); ++i){
		next.push_back(vec[i] - vec[i-1]);
	}
	if(forward){
		return next[next.size()-1] + traverse(next);
	}else{
		return next[0] - traverse(next, false);
	}
}

