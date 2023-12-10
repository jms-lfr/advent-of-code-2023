#include <iostream>
#include <fstream>
#include <vector>
#include <stdlib.h>
#include <algorithm>
#include <chrono>
#include <thread>

using std::size_t;
inline constexpr int reserveAmount = 30;

struct Range{
	long long destination; // [destination, destination+length)
	long long source; // [source, source+length)
	long long length;
	long long endSourceRange;
	long long function; // destination - source
};

void mapRangeToRange(std::vector<Range> &vec, const std::vector<Range> &_map);
void parseRanges(std::string, std::vector<Range> &);
std::vector<Range> createRangeVector(std::ifstream &);
void mapXtoRange(std::vector<long long> &, const std::vector<Range> &);

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
	// get seeds
	std::vector<long long> seeds;
	std::vector<Range> p2seeds;
	std::string line;
	getline(input, line); // seeds: n1, n2, ...
	{
		size_t pos = 0;
		auto counter = 0;
		auto lastSeed = 0ll;
		while((pos = line.find(' ', pos)) != std::string::npos){
			auto seed = strtoll(line.c_str() + pos, nullptr, 10);
			seeds.push_back(seed);
			if(counter & 1){
				p2seeds.push_back({0, lastSeed, seed, lastSeed + seed, 0});
			}else{
				lastSeed = seed;
			}
			++counter;
			++pos;
		}
	}
	getline(input, line); // blank line
	
	// create map vectors
	std::vector<Range> seedToSoil = createRangeVector(input);
	std::vector<Range> soilToFertilizer = createRangeVector(input);
	std::vector<Range> fertilizerToWater = createRangeVector(input);
	std::vector<Range> waterToLight = createRangeVector(input);
	std::vector<Range> lightToTemp = createRangeVector(input);
	std::vector<Range> tempToHumidity = createRangeVector(input);
	std::vector<Range> humidityToLoc = createRangeVector(input);
	input.close();

	mapXtoRange(seeds, seedToSoil);
	mapXtoRange(seeds, soilToFertilizer);
	mapXtoRange(seeds, fertilizerToWater);
	mapXtoRange(seeds, waterToLight);
	mapXtoRange(seeds, lightToTemp);
	mapXtoRange(seeds, tempToHumidity);
	mapXtoRange(seeds, humidityToLoc);

	mapRangeToRange(p2seeds, seedToSoil);
	mapRangeToRange(p2seeds, soilToFertilizer);
	mapRangeToRange(p2seeds, fertilizerToWater);
	mapRangeToRange(p2seeds, waterToLight);
	mapRangeToRange(p2seeds, lightToTemp);
	mapRangeToRange(p2seeds, tempToHumidity);
	mapRangeToRange(p2seeds, humidityToLoc);

	auto compRanges = [](const Range &l, const Range &r){ return l.source < r.source; };
	std::cout << "Day 5 Part 1: " << *(std::min_element(seeds.begin(), seeds.end())) << '\n';
	std::cout << "Day 5 Part 2: " << (*(std::min_element(p2seeds.begin(), p2seeds.end(), compRanges))).source << '\n';
	return 0;
}

void mapRangeToRange(std::vector<Range> &vec, const std::vector<Range> &maps){
	for(size_t i = 0; i < vec.size(); ++i){
		for(const auto &map : maps){
			// no overlap between range and map
			if(vec[i].endSourceRange <= map.source || vec[i].source >= map.endSourceRange) continue; 
			
			if(vec[i].source >= map.source && vec[i].endSourceRange <= map.endSourceRange){
				/*	best case scenario! --> full range within the map 
					don't have to split vec[i]
				*/
				vec[i].source += map.function;
				vec[i].endSourceRange += map.function;
				break; // don't map this updated range to anything else; it's been mapped
			}else if(vec[i].source >= map.source && vec[i].source < map.endSourceRange){ 
				/*	we know vec[i].endSourceRange > map.endSourceRange from previous
					may look like:
					range:	|-------------|
					map: |-------------|
					need to split vec[i] in two (in, out)
				*/ 
				// range1: map.endSourceRange+1, vec[i].endSourceRange (out)
				// range2: vec[i].source, map.endSourceRange (in)
				Range temp;
				temp.source = map.endSourceRange;
				temp.endSourceRange = vec[i].endSourceRange;
				vec.push_back(temp); // check if part outside the mapping can be mapped

				vec[i].source += map.function;
				vec[i].endSourceRange = map.endSourceRange + map.function;
				break; // done with v[i] since this part was in this mapping
			}else if(vec[i].endSourceRange <= map.endSourceRange && vec[i].endSourceRange > map.source){
				/* 	we know range.source < map.source from first if statement
					may look like:
					range: |-------------|
					map:	|-----------------|
					need to split vec[i] in two (out, in)
				*/
				// range1: vec[i].source, map.source-1 (out)
				// range2: map.source, vec[i].endSourceRange (in)
				Range temp;
				temp.source = vec[i].source;
				temp.endSourceRange = map.source;
				vec.push_back(temp); // check if part outside the mapping can be mapped

				vec[i].source = map.source + map.function;
				vec[i].endSourceRange += map.function;
				break; // done with v[i] since this part was in the mapping
			}else if (map.source >= vec[i].source && map.endSourceRange <= vec[i].endSourceRange){
				/*	the full map is between vec[i].source and vec[i].endSourceRange
					may look like:
					range: |-------------|
					map:	|----|
					need to split vec[i] in three (out, in, out)
				*/
				// range1: vec[i].source, map.source-1 (out)
				// range2: map.endSourceRange+1, vec[i].endSourceRange (out)
				// range3: map.source, map.endSourceRange (in)
				Range temp; 
				temp.source = vec[i].source;
				temp.endSourceRange = map.source;
				vec.push_back(temp); // check if part outside the mapping can be mapped

				temp.source = map.endSourceRange;
				temp.endSourceRange = vec[i].endSourceRange;
				vec.push_back(temp); // check if part outside the mapping can be mapped

				vec[i].source = map.source + map.function;
				vec[i].endSourceRange = map.endSourceRange + map.function;
				break; // done with v[i] since this part was in the mapping
			} else { 
				std::cout << "we should never be here\n";
				exit(1);
			}
		}
	}
}

void parseRanges(std::string str, std::vector<Range> &vec){
	// str assumed to be in the format x y z
	Range temp;
	temp.destination = strtoll(str.c_str(), nullptr, 10);

	size_t pos = 0;
	pos = str.find(' ', pos);
	temp.source = strtoll(str.c_str() + pos, nullptr, 10); 
	pos = str.find(' ', ++pos);
	temp.length = strtoll(str.c_str() + pos, nullptr, 10); 

	temp.function = temp.destination - temp.source; // source + function = destination;
	temp.endSourceRange = temp.source + temp.length;

	vec.push_back(temp);
}

std::vector<Range> createRangeVector(std::ifstream &input){
	std::vector<Range> vec;
	vec.reserve(reserveAmount);
	std::string line;
	getline(input, line); //"x-to-y map:"
	while(getline(input, line) && !line.empty()){
		parseRanges(line, vec);
	}
	return vec;
}

void mapXtoRange(std::vector<long long> &vec, const std::vector<Range> &map){
	for(auto &num : vec){
		for(auto &range : map){
			if(num >= range.source && num < range.endSourceRange){
				num += range.function;
				break;
			}
		}
	}
}
