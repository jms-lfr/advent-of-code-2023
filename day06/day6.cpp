#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cmath>

using std::size_t;

long long removeWhitespace(const std::string &line){
	long long result = 0ull;
	for(size_t i = 0; i < line.length(); ++i){
		if(isdigit(line[i])){
			result *= 10;
			result += line[i] - '0';
		}
	}
	return result; 
}

std::pair<long double, long double> quadraticFormula(long double a, long double b, long double c){
	auto discriminant = std::sqrt((b*b) - (4*a*c));
	return {(-b - discriminant) / (2*a), (-b + discriminant) / (2*a)};
}

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
	std::vector<int> times;
	std::vector<int> distances;
	std::string line;
	getline(input, line); // Time: x, y, ...
	std::stringstream ss(line);
	auto p2time = removeWhitespace(line);
	ss >> line; // remove Time:
	while(ss >> line){
		times.push_back(stoi(line));
	}
	getline(input, line);
	ss.clear();
	ss.str(line);
	auto p2distance = removeWhitespace(line);
	ss >> line; // remove Distance:
	while(ss >> line){
		distances.push_back(stoi(line));
	}
	input.close();

	std::vector<int> successfulHolds;
	// (time - x) * x = distance
	// -x^2 + timex - distance = 0
	for(size_t i = 0; i < times.size(); ++i){
		auto zeros = quadraticFormula(-1, times[i], -distances[i]);
		int successes = static_cast<int>(std::max(zeros.first, zeros.second)) - static_cast<int>(std::min(zeros.first, zeros.second));
		successfulHolds.push_back(successes);
	}

	auto resultP1 = 1;
	for(auto num : successfulHolds){
		resultP1 *= num;
	}

	auto zeros = quadraticFormula(-1, p2time, -p2distance);
	auto resultP2 = static_cast<long long>(std::max(zeros.first, zeros.second)) - static_cast<long long>(std::min(zeros.first, zeros.second));
	
	std::cout << "Day 6 Part 1: " << resultP1 << '\n';
	std::cout << "Day 6 Part 2: " << resultP2 << '\n';
	return 0;
}
