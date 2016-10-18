#include <iostream>
#include <vector>
#include <map>
#include <cstdlib>
using namespace std;

// The solved 8 puzzle
map<int, pair<int,int> > end_puzzle(void){
	map<int, pair<int,int> > puzzle_map;
	puzzle_map[0] = make_pair(0,0);
	puzzle_map[1] = make_pair(0,1);
	puzzle_map[2] = make_pair(0,2);
	puzzle_map[3] = make_pair(1,0);
	puzzle_map[4] = make_pair(1,1);
	puzzle_map[5] = make_pair(1,2);
	puzzle_map[6] = make_pair(2,0);
	puzzle_map[7] = make_pair(2,1);
	puzzle_map[8] = make_pair(2,2);
	return puzzle_map;
}

// Available directions for each of the 9 sections
// x is horizontal
// 1 is up, 2 is down, 3 is both
//
// y is vertical
// 1 is right, 2 is left, 3 is both
map<int, pair<int,int> > avail_directions(void){
	map<int, pair<int, int> > directions;
	directions[0] = make_pair(1,2);
	directions[1] = make_pair(3,2);
	directions[2] = make_pair(2,2);
	directions[3] = make_pair(1,3);
	directions[4] = make_pair(3,3);
	directions[5] = make_pair(2,3);
	directions[6] = make_pair(1,1);
	directions[7] = make_pair(3,1);
	directions[8] = make_pair(2,1);
	return directions;
}

// Initial b spot
// User input of 0

int initial_b_spot(vector<vector<int> > puzzle){
	for(int i = 0; i < 3; i++){
		for(int j = 0; j < 3; j++){
			if(puzzle.at(i).at(j) == 0)
				return (i * 3) + j;
		}
	}
	return 8;
}

// Returns the heuristic value according to the current puzzle

int heur(vector<vector<int> > puzzle, map<int, pair <int,int> > puzzle_map){
	int value = 0;
	for(int i = 0; i < 3; i++){
		for(int j = 0; j < 3; j++){
			int current = (i * 3) + j;
			pair<int,int> spot = puzzle_map[current];
			bool no_distance = false;
			if(puzzle.at(i).at(j) == 0)
				no_distance = true;
			pair<int,int> actual = puzzle_map[puzzle.at(i).at(j)-1];
			cout << spot.first << "," << spot.second << endl;
			cout << actual.first << "," << actual.second << endl;
			int distance = abs(actual.first - spot.first) + abs(actual.second - spot.second);
			if(no_distance)
				distance = 0;
			cout << "distance: " << distance << endl;
			value += distance;
		}
	}
	return value;
}

// Swaps the sections
// Is preformed when going down a node

vector<vector<int> > swap_places(vector<vector<int> > puzzle, map<int, pair<int,int> > puzzle_map, int& b_spot, int destination){
	const pair<int,int> foo = puzzle_map[b_spot];
	const pair<int,int> bar = puzzle_map[destination];
	int temp = puzzle.at(foo.first).at(foo.second);
	puzzle.at(foo.first).at(foo.second) = puzzle.at(bar.first).at(bar.second);
	puzzle.at(bar.first).at(bar.second) = temp;
	b_spot = destination;
	return puzzle;
}

void next_node(vector<vector<int> > puzzle, map<int, pair<int,int> > puzzle_map, map<int, pair<int,int> > directions, int b_spot){
	pair<int,int> curr_directions = directions[b_spot];
	int up=0, down=0, right=0, left=0;
	int b_spot2 = b_spot;
	vector<vector<int> > new_puzzle;
	vector<vector<int> > new_pizzle;
	/*
	for(int i = 0; i < 3; i++){
		for(int j = 0; j < 3; j++)
			cout << puzzle.at(i).at(j) << endl;
	}
	cout << "BIG" << b_spot << endl;
	int b_spot2 = b_spot;
	right = heur(new_puzzle=swap_places(puzzle, puzzle_map, b_spot, b_spot + 1), puzzle_map);
	for(int i = 0; i < 3; i++){
		for(int j = 0; j < 3; j++)
			cout << new_puzzle.at(i).at(j) << endl;
	}
	cout << "BIG" << b_spot2 << endl;
	left = heur(new_pizzle=swap_places(puzzle, puzzle_map, b_spot2, b_spot2 - 1), puzzle_map);
	for(int i = 0; i < 3; i++){
		for(int j = 0; j < 3; j++)
			cout << new_pizzle.at(i).at(j) << endl;
	}*/
	switch(curr_directions.first){
		case 1 : // right
			right = heur(swap_places(puzzle, puzzle_map, b_spot, b_spot + 1), puzzle_map);
			break;
		case 2 : // left
			left = heur(swap_places(puzzle, puzzle_map, b_spot, b_spot - 1), puzzle_map);
			break;
		case 3 : // both
			right = heur(new_puzzle=swap_places(puzzle, puzzle_map, b_spot, b_spot + 1), puzzle_map);
			for(int i = 0; i < 3; i++){
				for(int j = 0; j < 3; j++)
					cout << new_puzzle.at(i).at(j) << endl;
			}
			left = heur(new_pizzle=swap_places(puzzle, puzzle_map, b_spot2, b_spot2 - 1), puzzle_map);
			for(int i = 0; i < 3; i++){
				for(int j = 0; j < 3; j++)
					cout << new_pizzle.at(i).at(j) << endl;
			}
			break;
		default:
			break;
	}
	//switch(curr_directions.second)	
	return;
}

int main(){
	vector<vector<int> > puzzle;
	puzzle.resize(3, vector<int>(0,0));
	puzzle.at(0).push_back(2);
	puzzle.at(0).push_back(1);
	puzzle.at(0).push_back(3);
	puzzle.at(1).push_back(4);
	puzzle.at(1).push_back(5);
	puzzle.at(1).push_back(6);
	puzzle.at(2).push_back(7);
	puzzle.at(2).push_back(8);
	puzzle.at(2).push_back(0);

	int b_spot = initial_b_spot(puzzle);
	map<int, pair<int,int> >puzzle_map = end_puzzle();
	int value = heur(puzzle, puzzle_map);
	cout << "-------------------" << endl;
	cout << value << endl << endl;

	map<int, pair<int,int> > directions = avail_directions();
	//NORMALLY CANT SWAP
	//TEST ONLY
	puzzle = swap_places(puzzle, puzzle_map, b_spot, 7);
	value = heur(puzzle, puzzle_map);
	cout << "-------------------" << endl;
	cout << value << endl;
	cout << "b_spot: " << b_spot << endl;
	for(int i = 0; i < 3; i++){
		for(int j = 0; j < 3; j++)
			cout << puzzle.at(i).at(j) << endl;
	}
	value = heur(puzzle, puzzle_map);
	cout << "-------------------" << endl;
	cout << value << endl;
	next_node(puzzle, puzzle_map, directions, b_spot);
	return 0;
}
