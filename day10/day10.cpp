#include <iostream>
#include <fstream>
#include <vector>

// names for extended ASCII pipes
#define TOPLEFT 218
#define TOPRIGHT 191 
#define BOTTOMLEFT 192
#define BOTTOMRIGHT 217
#define VERTBAR 179
#define HORIBAR 196

using std::size_t;

enum Direction {North, East, South, West};

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

	std::vector<std::string> pipes;
	std::pair<size_t, size_t> startPosition(-1, -1);
	std::string line;
	{
		bool notFoundStart = true;
		size_t temp;
		for(auto i = 0ull; getline(input, line); ++i){
			if( notFoundStart && (temp = line.find('S')) != std::string::npos ){
				startPosition.first = i;
				startPosition.second = temp;
				notFoundStart = false;
			}
			pipes.push_back(line);
		}
		input.close();
	}
	
	// solve the problem
	long long resultP1 = 0;
	long long resultP2 = 0;

	// *******************************
	// hardcoded for my input where S = '|'
	std::pair<size_t, size_t> currPos(startPosition.first-1, startPosition.second);
	Direction dir = North;
	// *******************************
	
	resultP1 += 1; // count previous move

	while(currPos != startPosition){
		resultP1 += 1;
		size_t savex = currPos.first, savey = currPos.second; // to overwrite old position with x
		unsigned char overwrite = 'x';
		switch(pipes[currPos.first][currPos.second]){
			case '|':
				currPos.first += dir == North ? -1 : 1;
				overwrite = VERTBAR;
				break;
			case '-':
				currPos.second += dir == West ? -1 : 1;
				overwrite = HORIBAR;
				break;
			case 'L':
				if(dir == South){
					currPos.second += 1;
					dir = East;
				}else{
					currPos.first -= 1;
					dir = North;
				}
				overwrite = BOTTOMLEFT;
				break;
			case 'J':
				if(dir == South){
					currPos.second -= 1;
					dir = West;
				}else{
					currPos.first -= 1;
					dir = North;
				}
				overwrite = BOTTOMRIGHT;
				break;
			case '7':
				if(dir == North){
					currPos.second -= 1;
					dir = West;
				}else{
					currPos.first += 1;
					dir = South;
				}
				overwrite = TOPRIGHT;
				break;
			case 'F':
				if(dir == North){
					currPos.second += 1;
					dir = East;
				}else{
					currPos.first += 1;
					dir = South;
				}
				overwrite = TOPLEFT;
				break;
			default:
				std::cout << "This should never happen. " << resultP1 << '\n';
				//std::cout << startPosition.first << ' ' << startPosition.second << '\n';
				exit(1);
		}
		pipes[savex][savey] = overwrite;
	}

	for(auto& line : pipes){
		bool inside = false;
		unsigned char last = 0u;
		for(size_t i = 0ull; i < line.length(); ++i){
			switch(static_cast<unsigned char>(line[i])){
				case 'S': // **HARDCODED**: S is a vertical pipe in my input
				case VERTBAR:
					inside = !inside;
					//resultP2 += temp;
					//temp = 0;
					break;
				case HORIBAR:
					break;
				case TOPLEFT:
					last = TOPLEFT;
					break;
				case BOTTOMLEFT:
					last = BOTTOMLEFT;
					break;
				case TOPRIGHT:
					if(last == BOTTOMLEFT){
						inside = !inside;
					}
					break;
				case BOTTOMRIGHT:
					if(last == TOPLEFT){
						inside = !inside;
					}
					break;
				case 'x':
				case '*':
					std::cout << "This should never happen. " << i << '\n';
					exit(1);
					break;
				default:
					if(inside){
						++resultP2;
						line[i] = '*';
					}
					break;
			}
		}
		//std::cout << line << '\n';
	}
	//std::cout << std::flush;
	
	std::cout << "Day 10 Part 1: " << resultP1/2 << '\n';
	std::cout << "Day 10 Part 2: " << resultP2 << '\n';
	return 0;
}
