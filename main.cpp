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

int misplaced_tiles(vector<vector<int> > puzzle){
	int value = 0;
	for(int i = 0; i < 3; i++){
		for(int j = 0; j < 3; j++){
			int current = (i * 3) + j + 1;
			bool not_tile = false;
			if(puzzle.at(i).at(j) == 0)
				not_tile = true;
			int actual = puzzle.at(i).at(j);
			//cout << spot.first << "," << spot.second << endl;
			//cout << actual.first << "," << actual.second << endl;
			if(actual != current){
				if(!not_tile)
					value++;
			}
				
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
	Node() : direction(""), index(0), distance(-1), b_spot(0){}
	string direction;
	int index;
	int distance;
	int b_spot;
	vector<vector<int> > puzzle;
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
vector<Node> popped;
int go_back = 0;

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
			node_right.distance = heur(node_right.puzzle=swap_places(puzzle, puzzle_map, b_spot1, b_spot1 + 1), puzzle_map);
			break;
		case 2 : // left
			node_left.direction = "L";
			node_left.distance = heur(node_left.puzzle=swap_places(puzzle, puzzle_map, b_spot2, b_spot2 - 1), puzzle_map);
			break;
		case 3 : // both
			node_right.direction = "R";
			node_right.distance = heur(node_right.puzzle=swap_places(puzzle, puzzle_map, b_spot1, b_spot1 + 1), puzzle_map);
			
			/*
			for(int i = 0; i < 3; i++){
				for(int j = 0; j < 3; j++)
					cout << new_puzzle.at(i).at(j) << endl;
			}*/
			node_left.direction = "L";
			node_left.distance = heur(node_left.puzzle=swap_places(puzzle, puzzle_map, b_spot2, b_spot2 - 1), puzzle_map);
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
			node_up.distance = heur(node_up.puzzle=swap_places(puzzle, puzzle_map, b_spot3, b_spot3 - 3), puzzle_map);
			break;
		case 2 : // down
			node_down.direction = "D";
			node_down.distance = heur(node_down.puzzle=swap_places(puzzle, puzzle_map, b_spot4, b_spot4 + 3), puzzle_map);
			break;
		case 3 : // both
			node_up.direction = "U";
			node_up.distance = heur(node_up.puzzle=swap_places(puzzle, puzzle_map, b_spot3, b_spot3 - 3), puzzle_map);
			node_down.direction = "D";
			node_down.distance = heur(node_down.puzzle=swap_places(puzzle, puzzle_map, b_spot4, b_spot4 + 3), puzzle_map);
			break;
		default:
			break;

	}
	bool went_right = false;
	bool went_left = false;
	bool went_up = false;
	bool went_down = false;
	for(int i = 0; i < popped.size(); i++){
		/*
		cout << "--------------" << endl;
		for(int k = 0; k < 3; k++){
			for(int j = 0; j < 3; j++)
				cout << popped.at(i).puzzle.at(k).at(j) << endl;
		}*/
		/*
		cout << "=============" << endl;
		for(int k = 0; k < 3; k++){
			for(int j = 0; j < 3; j++)
				cout << node_down.puzzle.at(k).at(j) << endl;
		}
		cout << "--------------" << endl;
		*/
		if(popped.at(i).puzzle == node_right.puzzle && popped.at(i).direction == node_right.direction)
			went_right = true;
		else if(node_right.distance != -1 && i == popped.size()-1 && !went_right)
			tree.push(node_right);
		if(popped.at(i).puzzle == node_left.puzzle && popped.at(i).direction == node_left.direction)
			went_left = true;
		else if(node_left.distance != -1 && i == popped.size()-1 && !went_left)
			tree.push(node_left);
		if(popped.at(i).puzzle == node_up.puzzle && popped.at(i).direction == node_up.direction)
			went_up = true;
		else if(node_up.distance != -1 && i == popped.size()-1 && !went_up)
			tree.push(node_up);
		if(popped.at(i).puzzle == node_down.puzzle && popped.at(i).direction == node_down.direction)
			went_down = true;
		else if(node_down.distance != -1 && i == popped.size()-1 && !went_down)
			tree.push(node_down);
	}
	/*
	if(node_left.distance != -1)
		tree.push(node_left);
	if(node_up.distance != -1)
		tree.push(node_up);
	if(node_down.distance != -1)
		tree.push(node_down);
	*/
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
	if(tree.empty()){
		cout << "back" << endl;
		if(go_back == 0){
			ultimate_puzzle = popped.at(0).puzzle;
			b_spot = popped.at(0).b_spot;
			go_back = popped.size();
		}
		else if(level == popped.size()){
			ultimate_puzzle = popped.at(0).puzzle;
			b_spot = popped.at(0).b_spot;
			go_back = popped.size();
		}
		else if(go_back > 0){
			ultimate_puzzle = popped.at(level).puzzle;
			b_spot = popped.at(level).b_spot;
			go_back = level;
		}
	}
	
	while(!tree.empty()){
		string dir = tree.top().direction;
		printf("(Direction:%s, Distance: %d, Depth: %d)\n", dir.c_str(), tree.top().distance, tree.top().index);
		if(k)
			real_tree.push_back(tree.top());
		else
			popped.push_back(tree.top());
		tree.pop();
		k++;
	}
	/*
	reverse(real_tree.begin(), real_tree.end());
	for(int i = 0; i < real_tree.size(); i++){
		string dir = real_tree.at(i).direction;
		cout << "wowowo" << endl;
		printf("(Direction:%s, Distance: %d, Depth: %d)\n", dir.c_str(), real_tree.at(i).distance, real_tree.at(i).index);
	}*/
	
	return ultimate_puzzle;
}

void BFS(vector<vector<int> > puzzle, map<int, pair<int,int> > puzzle_map, map<int, pair<int,int> > directions, int b_spot, int level){
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
	node_right.index = level + 1;
	node_left.index = level + 1;
	node_up.index = level + 1;
	node_down.index = level + 1;
	vector<vector<int> > ultimate_puzzle;
	priority_queue<Node, vector<Node>, least_distance> tree_copy;
	switch(curr_directions.first){
		case 1 : // right
			node_right.direction = "R";
			node_right.distance = heur(node_right.puzzle=swap_places(puzzle, puzzle_map, b_spot1, b_spot1 + 1), puzzle_map);
			break;
		case 2 : // left
			node_left.direction = "L";
			node_left.distance = heur(node_left.puzzle=swap_places(puzzle, puzzle_map, b_spot2, b_spot2 - 1), puzzle_map);
			break;
		case 3 : // both
			node_right.direction = "R";
			node_right.distance = heur(node_right.puzzle=swap_places(puzzle, puzzle_map, b_spot1, b_spot1 + 1), puzzle_map);
			node_left.direction = "L";
			node_left.distance = heur(node_left.puzzle=swap_places(puzzle, puzzle_map, b_spot2, b_spot2 - 1), puzzle_map);
			break;
		default:
			break;
	}
	switch(curr_directions.second){
		case 1 : // up
			node_up.direction = "U";
			node_up.distance = heur(node_up.puzzle=swap_places(puzzle, puzzle_map, b_spot3, b_spot3 - 3), puzzle_map);
			break;
		case 2 : // down
			node_down.direction = "D";
			node_down.distance = heur(node_down.puzzle=swap_places(puzzle, puzzle_map, b_spot4, b_spot4 + 3), puzzle_map);
			break;
		case 3 : // both
			node_up.direction = "U";
			node_up.distance = heur(node_up.puzzle=swap_places(puzzle, puzzle_map, b_spot3, b_spot3 - 3), puzzle_map);

			node_down.direction = "D";
			node_down.distance = heur(node_down.puzzle=swap_places(puzzle, puzzle_map, b_spot4, b_spot4 + 3), puzzle_map);
			break;
		default:
			break;

	}
	node_right.b_spot = b_spot1;
	node_left.b_spot = b_spot2;
	node_up.b_spot = b_spot3;
	node_down.b_spot = b_spot4;
	if(node_right.distance != -1)
		real_tree.push_back(node_right);
	if(node_left.distance != -1)
		real_tree.push_back(node_left);
	if(node_up.distance != -1)
		real_tree.push_back(node_up);
	if(node_down.distance != -1)
		real_tree.push_back(node_down);
	//cout << "-----------------\n" << node_right.direction << endl << node_right.distance << endl;
	//cout << "-----------------\n" << node_left.direction << endl << node_left.distance << endl;
	//cout << "-----------------\n" << node_up.direction << endl << node_up.distance << endl;
	//cout << tree.top().direction << endl << tree.top().distance << endl;

}

vector<vector<int> > a_star_misplaced(vector<vector<int> > puzzle, map<int, pair<int,int> > puzzle_map, map<int, pair<int,int> > directions, int& b_spot, int level){
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
	vector<vector<int> > ultimate_puzzle;
	priority_queue<Node, vector<Node>, least_distance> tree_copy;
	switch(curr_directions.first){
		case 1 : // right
			node_right.direction = "R";
			node_right.distance = misplaced_tiles(swap_places(puzzle, puzzle_map, b_spot1, b_spot1 + 1));
			break;
		case 2 : // left
			node_left.direction = "L";
			node_left.distance = misplaced_tiles(swap_places(puzzle, puzzle_map, b_spot2, b_spot2 - 1));
			break;
		case 3 : // both
			node_right.direction = "R";
			node_right.distance = misplaced_tiles(swap_places(puzzle, puzzle_map, b_spot1, b_spot1 + 1));
			
			node_left.direction = "L";
			node_left.distance = misplaced_tiles(swap_places(puzzle, puzzle_map, b_spot2, b_spot2 - 1));
			break;
		default:
			break;
	}
	switch(curr_directions.second){
		case 1 : // up
			node_up.direction = "U";
			node_up.distance = misplaced_tiles(swap_places(puzzle, puzzle_map, b_spot3, b_spot3 - 3));
			break;
		case 2 : // down
			node_down.direction = "D";
			node_down.distance = misplaced_tiles(swap_places(puzzle, puzzle_map, b_spot4, b_spot4 + 3));
			break;
		case 3 : // both
			node_up.direction = "U";
			node_up.distance = misplaced_tiles(swap_places(puzzle, puzzle_map, b_spot3, b_spot3 - 3));
			node_down.direction = "D";
			node_down.distance = misplaced_tiles(swap_places(puzzle, puzzle_map, b_spot4, b_spot4 + 3));
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
	cout << "POPPED\n" << endl;
	reverse(real_tree.begin(), real_tree.end());
	for(int i = 0; i < real_tree.size(); i++){
		string dir = real_tree.at(i).direction;
		printf("(Direction:%s, Distance: %d, Depth: %d)\n", dir.c_str(), real_tree.at(i).distance, real_tree.at(i).index);
	}
	
	return ultimate_puzzle;
}


int main(){
	vector<vector<int> > puzzle;
	puzzle.resize(3, vector<int>(0,0));
	puzzle.at(0).push_back(1);
	puzzle.at(0).push_back(5);
	puzzle.at(0).push_back(3);
	puzzle.at(1).push_back(4);
	puzzle.at(1).push_back(0);
	puzzle.at(1).push_back(6);
	puzzle.at(2).push_back(7);
	puzzle.at(2).push_back(2);
	puzzle.at(2).push_back(8);

	int level = 0;

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
	
	Node root;
	root.puzzle = puzzle;
	root.b_spot = b_spot;
	root.index = 0;
	popped.push_back(root);

	while(value != 0){
	//for(int k = 0; k < 10000; k++){
		puzzle = next_node(puzzle, puzzle_map, directions, b_spot, level);
		if(puzzle == root.puzzle)
			level = 0;
		value = heur(puzzle, puzzle_map);
		
		for(int i = 0; i < 3; i++){
			for(int j = 0; j < 3; j++)
				cout << puzzle.at(i).at(j) << endl;
		}
		level += 1;
		cout << "FINAL: " << value << endl;
		cout << level << endl;
		cout << "POPPED_SZ: " << popped.size() << endl;
	}

	/*
	Node root;
	root.puzzle = puzzle;
	root.b_spot = b_spot;
	root.index = 0;
	real_tree.push_back(root);
	while(value != 0){
		for(int i = 0; i < real_tree.size(); i++){
			string dir = real_tree.at(i).direction;
			cout << "wowowo" << endl;
			printf("(Direction:%s, Distance: %d, Depth: %d)\n", dir.c_str(), real_tree.at(i).distance, real_tree.at(i).index);
		}
		BFS(real_tree.at(0).puzzle, puzzle_map, directions, real_tree.at(0).b_spot, real_tree.at(0).index);
		cout << "\n\n\n";
		real_tree.erase(real_tree.begin());
		for(int i = 0; i < real_tree.size(); i++){
			string dir = real_tree.at(i).direction;
			cout << "wowowo" << endl;
			printf("(Direction:%s, Distance: %d, Depth: %d)\n", dir.c_str(), real_tree.at(i).distance, real_tree.at(i).index);
		}
		value = heur(real_tree.at(0).puzzle, puzzle_map);
		cout << "FINAL: " << value << endl;
		cout << "LEVEL: " << level << endl;
		cout << endl;

	}*/
	/*
	cout << misplaced_tiles(puzzle) << endl;
	while(value != 0){
	//for(int k = 0; k < 5; k++){
		puzzle = a_star_misplaced(puzzle, puzzle_map, directions, b_spot, level);
		value = misplaced_tiles(puzzle);
		for(int i = 0; i < 3; i++){
			for(int j = 0; j < 3; j++)
				cout << puzzle.at(i).at(j) << endl;
		}
		level += 1;
		cout << "FINAL: " << value << endl;
	}*/
	return 0;
}










