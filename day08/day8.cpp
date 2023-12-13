#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <numeric>

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

	// solve the problem
	size_t resultP1 = 0;
	size_t resultP2 = 0;
	std::string directions;
	getline(input, directions); // just index the string for directions
	std::unordered_map<std::string, std::pair<std::string, std::string>> nodes;
	std::vector<std::pair<std::string, size_t>> p2currNodes;
	std::string line;
	getline(input, line); // remove blank line
	while(getline(input, line)){
		std::string temp = line.substr(0, 3);
		nodes[temp] = std::pair<std::string, std::string>(line.substr(7, 3), line.substr(12, 3));
		if(temp[2] == 'A'){ // we know temp.length() == 3
			p2currNodes.push_back({temp, 0ull});
		}
	}
	input.close();

	std::string currNode("AAA");
	for(size_t i = 0;; ++i){
		auto pos = i % directions.length();
		if(directions[pos] == 'L'){
			currNode = nodes[currNode].first;
		}else{ // directions[i] == 'R'
			currNode = nodes[currNode].second;
		}
		if(currNode == "ZZZ"){
			resultP1 = i+1; // we traverse immediately, so add the current path we just took
			break;
		}
	}

	// we know every node ends with A at the start
	auto foundCount = 0;
	for(size_t i = 0;; ++i){
		auto pos = i % directions.length();
		for(auto &node : p2currNodes){
			if(directions[pos] == 'L'){
				node.first = nodes[node.first].first;
			}else{
				node.first = nodes[node.first].second;
			}
			if(!node.second && node.first[2] == 'Z'){
				node.second = i+1; // we traverse immediately, so add the current path we just took
				if(++foundCount == 6){
					break;
				}
			}
		}
		if(foundCount == 6){
			break;
		}
	}
	
	resultP2 = p2currNodes[0].second;
	for(size_t i = 1; i < p2currNodes.size(); ++i){
		resultP2 = std::lcm(resultP2, p2currNodes[i].second);
	}

	std::cout << "Day 8 Part 1: " << resultP1 << '\n';
	std::cout << "Day 8 Part 2: " << resultP2 << '\n';
	return 0;
}
