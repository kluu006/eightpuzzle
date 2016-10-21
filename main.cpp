#include <iostream>
#include <vector>
#include <map>
#include <cstdlib>
#include <queue>
#include <stdio.h>
#include <string>
#include <algorithm>

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
			//cout << spot.first << "," << spot.second << endl;
			//cout << actual.first << "," << actual.second << endl;
			int distance = abs(actual.first - spot.first) + abs(actual.second - spot.second);
			if(no_distance)
				distance = 0;
			//cout << "distance: " << distance << endl;
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

struct Node {
	Node() : direction(""), index(0), distance(-1){}
	string direction;
	int index;
	int distance;
};

struct least_distance{
	bool operator()(const Node& left, const Node& right) const{
		if(left.index == right.index)
			return left.distance > right.distance;
		else if(left.index > right.index)
			return right.distance;
	}
};

priority_queue<Node, vector<Node>, least_distance> tree;
vector<Node> real_tree;

vector<vector<int> > next_node(vector<vector<int> > puzzle, map<int, pair<int,int> > puzzle_map, map<int, pair<int,int> > directions, int& b_spot, int level){
	pair<int,int> curr_directions = directions[b_spot];
	int up=0, down=0, right=0, left=0;
	int b_spot1 = b_spot;
	int b_spot2 = b_spot;
	int b_spot3 = b_spot;
	int b_spot4 = b_spot;
	Node node_right; 
	Node node_left;
	Node node_up;
	Node node_down;
	node_right.index = level;
	node_left.index = level;
	node_up.index = level;
	node_down.index = level;
	vector<vector<int> > new_puzzle;
	vector<vector<int> > new_pizzle;
	vector<vector<int> > ultimate_puzzle;
	priority_queue<Node, vector<Node>, least_distance> tree_copy;
	switch(curr_directions.first){
		case 1 : // right
			node_right.direction = "R";
			node_right.distance = heur(swap_places(puzzle, puzzle_map, b_spot1, b_spot1 + 1), puzzle_map);
			break;
		case 2 : // left
			node_left.direction = "L";
			node_left.distance = heur(swap_places(puzzle, puzzle_map, b_spot2, b_spot2 - 1), puzzle_map);
			break;
		case 3 : // both
			node_right.direction = "R";
			node_right.distance = heur(new_puzzle=swap_places(puzzle, puzzle_map, b_spot1, b_spot1 + 1), puzzle_map);
			
			/*
			for(int i = 0; i < 3; i++){
				for(int j = 0; j < 3; j++)
					cout << new_puzzle.at(i).at(j) << endl;
			}*/
			node_left.direction = "L";
			node_left.distance = heur(new_pizzle=swap_places(puzzle, puzzle_map, b_spot2, b_spot2 - 1), puzzle_map);
			/*
			for(int i = 0; i < 3; i++){
				for(int j = 0; j < 3; j++)
					cout << new_pizzle.at(i).at(j) << endl;
			}*/
			break;
		default:
			break;
	}
	switch(curr_directions.second){
		case 1 : // up
			node_up.direction = "U";
			node_up.distance = heur(swap_places(puzzle, puzzle_map, b_spot3, b_spot3 - 3), puzzle_map);
			break;
		case 2 : // down
			node_down.direction = "D";
			node_down.distance = heur(swap_places(puzzle, puzzle_map, b_spot4, b_spot4 + 3), puzzle_map);
			break;
		case 3 : // both
			node_up.direction = "U";
			node_up.distance = heur(new_puzzle=swap_places(puzzle, puzzle_map, b_spot3, b_spot3 - 3), puzzle_map);
			for(int i = 0; i < 3; i++){
				for(int j = 0; j < 3; j++)
					cout << new_puzzle.at(i).at(j) << endl;
			}
			node_down.direction = "D";
			node_down.distance = heur(new_pizzle=swap_places(puzzle, puzzle_map, b_spot4, b_spot4 + 3), puzzle_map);
			for(int i = 0; i < 3; i++){
				for(int j = 0; j < 3; j++)
					cout << new_pizzle.at(i).at(j) << endl;
			}
			break;
		default:
			break;

	}
	if(node_right.distance != -1)
		tree.push(node_right);
	if(node_left.distance != -1)
		tree.push(node_left);
	if(node_up.distance != -1)
		tree.push(node_up);
	if(node_down.distance != -1)
		tree.push(node_down);
	//cout << "-----------------\n" << node_right.direction << endl << node_right.distance << endl;
	//cout << "-----------------\n" << node_left.direction << endl << node_left.distance << endl;
	//cout << "-----------------\n" << node_up.direction << endl << node_up.distance << endl;
	//cout << tree.top().direction << endl << tree.top().distance << endl;
	if(tree.top().direction == "R")
		ultimate_puzzle = swap_places(puzzle, puzzle_map, b_spot, b_spot + 1);
	else if(tree.top().direction == "L")
		ultimate_puzzle = swap_places(puzzle, puzzle_map, b_spot, b_spot - 1);
	else if(tree.top().direction == "U")
		ultimate_puzzle = swap_places(puzzle, puzzle_map, b_spot, b_spot - 3);
	else if(tree.top().direction == "D")
		ultimate_puzzle = swap_places(puzzle, puzzle_map, b_spot, b_spot + 3);
		
	//cout << tree_level.top().direction << endl << tree_level.top().distance << endl;
	tree_copy = tree;
	
	bool one = true;
	int k = 0;
	while(!tree.empty()){
		string dir = tree.top().direction;
		printf("(Direction:%s, Distance: %d, Depth: %d)\n", dir.c_str(), tree.top().distance, tree.top().index);
		if(k)
			real_tree.push_back(tree.top());
		tree.pop();
		k++;
	}
	reverse(real_tree.begin(), real_tree.end());
	for(int i = 0; i < real_tree.size(); i++){
		string dir = real_tree.at(i).direction;
		cout << "wowowo" << endl;
		printf("(Direction:%s, Distance: %d, Depth: %d)\n", dir.c_str(), real_tree.at(i).distance, real_tree.at(i).index);
	}
	
	return ultimate_puzzle;
}

int main(){
	vector<vector<int> > puzzle;
	puzzle.resize(3, vector<int>(0,0));
	puzzle.at(0).push_back(1);
	puzzle.at(0).push_back(2);
	puzzle.at(0).push_back(3);
	puzzle.at(1).push_back(4);
	puzzle.at(1).push_back(8);
	puzzle.at(1).push_back(0);
	puzzle.at(2).push_back(7);
	puzzle.at(2).push_back(6);
	puzzle.at(2).push_back(5);

	int level = 1;

	int b_spot = initial_b_spot(puzzle);
	map<int, pair<int,int> >puzzle_map = end_puzzle();
	int value = heur(puzzle, puzzle_map);
	//cout << "-------------------" << endl;
	//cout << value << endl << endl;

	map<int, pair<int,int> > directions = avail_directions();
	//NORMALLY CANT SWAP
	//TEST ONLY
	//puzzle = swap_places(puzzle, puzzle_map, b_spot, 7);
	value = heur(puzzle, puzzle_map);

	//cout << "-------------------" << endl;
	//cout << value << endl;
	cout << "b_spot: " << b_spot << endl;
	/*
	for(int i = 0; i < 3; i++){
		for(int j = 0; j < 3; j++)
			cout << puzzle.at(i).at(j) << endl;
	}*/
	//value = heur(puzzle, puzzle_map);
	//cout << "-------------------" << endl;
	//cout << value << endl;
	
	while(value != 0){
	//for(int k = 0; k < 5; k++){
		puzzle = next_node(puzzle, puzzle_map, directions, b_spot, level);
		value = heur(puzzle, puzzle_map);
		for(int i = 0; i < 3; i++){
			for(int j = 0; j < 3; j++)
				cout << puzzle.at(i).at(j) << endl;
		}
		level += 1;
		cout << "FINAL: " << value << endl;
	}
	return 0;
}
