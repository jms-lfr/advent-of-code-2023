#include <iostream>
#include <fstream>
#include <map>
#include <algorithm>
#include <vector>
#include <limits.h>

using std::size_t;

enum HandType {
	highCard, onePair, twoPair, threeOfaKind, fullHouse, fourOfaKind, fiveOfaKind
};

struct Card {
	std::string hand;
	int bid;
	HandType type;
	HandType p2type;
};

void setCardType(Card &card, bool part2 = false){
	std::map<char, int> counts;
	for(auto &c : card.hand){
		counts[c] += 1;
	}
	auto max = INT_MIN;
	auto maxCount = 0;
	auto min = INT_MAX;
	for(auto &[key, value] : counts){
		if(value == max){
			++maxCount;
		} else if(value > max){ 
			max = value;
			maxCount = 1;
		} 
		if(value < min){
			min = value;
		}
	}
	switch(max){
		case 5:
			card.type = HandType::fiveOfaKind;
			break;
		case 4:
			card.type = part2 && counts['W'] ? HandType::fiveOfaKind : HandType::fourOfaKind;
			break;
		case 3:
			if(min == 2){
				// we know min is 2, so if there are 'W's, we have 3+2=5
				// otherwise, we just have 3 and 2 separately
				card.type = part2 && counts['W'] ? HandType::fiveOfaKind : HandType::fullHouse;
			}else{
				switch(counts['W']){
					case 3: // depends on other two cards
						card.type = (min == 2) ? HandType::fiveOfaKind : HandType::fourOfaKind;
						break;
					case 2: // we have three of a kind already, so 3+2=5
						card.type = HandType::fiveOfaKind;
						break;
					case 1: // we have three of a kind already, so 3+1=4
						card.type = HandType::fourOfaKind;
						break;
					case 0: 
					default:
						card.type = HandType::threeOfaKind;
						break;
				}
			}
			break;
		case 2:
			if(maxCount > 1){
				if(part2 && counts['W']){
					// if 'W' is a pair, add the 'W's to the second pair, making 4
					// otherwise, there must be only 1 'W', so we have 3 of one card and 2 of another,
					// making a full house
					card.type = counts['W'] == 2 ? HandType::fourOfaKind : HandType::fullHouse;
				}else{
					card.type = HandType::twoPair;
				}
			}else{ 
				// either the pair is of 'W' which we can combine with one other,
				// there is one 'W' which we can combine with the pair, 
				// there are no 'W' so we have one pair
				card.type = part2 && counts['W'] ? HandType::threeOfaKind : HandType::onePair;
			}
			break;
		case 1:
		default: // either combine a single card with a 'W', or there are no 'W'
			card.type = part2 && counts['W'] ? HandType::onePair : HandType::highCard;
	}
}

bool compareCards(const Card &left, const Card &right){
	if(left.type > right.type) return false;
	if(left.type < right.type) return true;
	
	// left.length() == 5 == right.length()
	for(auto i = 0; i < 5; ++i){
		if(left.hand[i] > right.hand[i]) return false;
		if(left.hand[i] < right.hand[i]) return true;
	}
	/*if(left.bid > right.bid) return false;
	if(left.bid < right.bid) return true;*/
	return false;
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
	auto resultP1 = 0ll;
	auto resultP2 = 0ll;
	std::string line;
	std::vector<Card> cards;
	cards.reserve(1000);
	while(getline(input, line)){
		Card temp;
		temp.hand = line.substr(0, 5);
		for(auto i = 0; i < 5; ++i){ // temp.hand.length() == 5
			switch(temp.hand[i]){ 
				// replace letters for sorting purposes 
				// we want 'A' > 'K' > 'Q' > 'J' > 'T' > '9' > ...
				// PART 2: 'J' should be the LOWEST (adjusted later)
				case 'A':
					temp.hand[i] = 'Z';
					break;
				case 'K':
					temp.hand[i] = 'Y';
					break;
				case 'Q':
					temp.hand[i] = 'X';
					break;
				case 'J':
					temp.hand[i] = 'W';
					break;
				case 'T':
					temp.hand[i] = 'V';
					break;
				default:
					break;
			}
		}
		temp.bid = stoi(line.substr(6));
		setCardType(temp);
		cards.push_back(temp);
	}
	
	std::stable_sort(cards.begin(), cards.end(), compareCards);

	auto rank = 1;
	for(auto &c : cards){ // calculate P1, prepare for P2
		resultP1 += (c.bid * rank);
		++rank;
		setCardType(c, true);
		for(int i = 0; i < 5; ++i){ // c.length() == 5
			if(c.hand[i] == 'W'){
				c.hand[i] = '-'; // '-' < '2'
			}
		}
	}

	std::sort(cards.begin(), cards.end(), compareCards);
	rank = 1;
	for(const auto &c : cards){
		resultP2 += (c.bid * rank);
		++rank;
	}

	std::cout << "Day 7 Part 1: " << resultP1 << '\n';
	std::cout << "Day 7 Part 2: " << resultP2 << '\n';
	return 0;
}
