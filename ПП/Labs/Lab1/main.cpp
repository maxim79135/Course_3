#include <iostream>
#include <cstdlib>
#include <cmath>
#include <ctime>
#include <vector>
#include "Map.h"
#include "BinTree.h"
#include "State.h"
#include <ctime>
#include <limits>

#define TESTS 10

using namespace std;


bool check (Map*);
void printMap(Map*);

Map* generateMap(int lines, int cols) {
	int len = lines * cols;
	Map* map = new Map(lines, cols);

	for (int i = 0; i < len; ++i)
	{
		map->map[i] = i + 1;
	}
	map->map[len - 1] = 0;
	int i = 0;
	int shift_pos;
	while (i<=len*20) {
		int zero = map->find(0);
		shift_pos = rand() % 4;
		switch (shift_pos){
		case 0:
			if (zero / map->getCols() != 0) {
				map = map->shift(shift_pos);
				i++;
			}
			continue;
		case 1:
			if (zero % map->getCols() != map->getCols() - 1) {
				map = map->shift(shift_pos);
				i++;
			}
			continue;
		case 2:
			if (zero / map->getCols() != map->getLines() - 1) {
				map = map->shift(shift_pos);
				i++;
			}
			continue;
		case 3:
			if (zero % map->getCols() != 0) {
				map = map->shift(shift_pos);
				i++;
			}
			continue;
		}
	}

	return map;
}

void printMap(Map* map) {
	std::cout << std::endl;
	for (int i = 0; i < map->lines; ++i) {
		for (int j = 0; j < map->cols; ++j) {
			std::cout << map->map[i*map->cols + j] << '\t';
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
}

vector<State*> a(Map* map) {

	BinTree open = BinTree(new State(map, NULL));
	BinTree close = BinTree();
	State* min = open.min();

	close.add(min);
	open.del(min);

	for (; min->getCost() != 0; min = open.min(), close.add(min), open.del(min))
	{
		int zero = min->getMap()->find(0);

		if (zero / map->getCols() != 0) {
			State* s = new State(min->getMap()->shift(0), min);
			if ((open.find(s) == NULL) && (close.find(s) == NULL)) {
				open.add(s);
			}
		}

		if (zero % map->getCols() != map->getCols() - 1) {
			State* s = new State(min->getMap()->shift(1), min);
			if ((open.find(s) == NULL) && (close.find(s) == NULL)) {
				open.add(s);
			}
		}

		if (zero / map->getCols() != map->getLines() - 1) {
			State* s = new State(min->getMap()->shift(2), min);
			if ((open.find(s) == NULL) && (close.find(s) == NULL)) {
				open.add(s);
			}
		}

		if (zero % map->getCols() != 0) {
			State* s = new State(min->getMap()->shift(3), min);
			if ((close.find(s) == NULL) && (open.find(s) == NULL)) {
				open.add(s);
			}
		}

	}
	std::vector<State*> solution;

	State* s = min;
	do
	{
		solution.push_back(s);
		s = s->getParent();
	} while (s != NULL);

	return solution;
}


int main(int argc, char const *argv[]) {
	int lines, cols;
	Map* map;

	do {
		system("cls");
		cout << "Enter field sizes: " << endl;
		cin >> lines >> cols;
		cin.clear();
		if (cin.good() == false || cols < 2 || lines < 2) {
			system("cls");
			std::cout << "The size of the field is wrong" << std::endl;
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			getchar();
		}
	} while (cols*lines <= 4);

	double t = 0;
	std::vector<State*> ans;

	for (int i = 0; i < TESTS; i++) {
		srand(i);
		map = generateMap(lines, cols);

		printMap(map);

		std::cout << "\n" << "Case #" << i + 1 << ": ";
		clock_t time = clock();
		ans = a(map);
		time = clock() - time;

		std::cout << "\n" << "Solution:";

		for (int k = ans.size()-1; k >= 0; k--) {
			printMap(ans[k]->getMap());
		}

		std::cout << "Time:" << (double)time / CLOCKS_PER_SEC << std::endl;
		t += (double)time / CLOCKS_PER_SEC;

	}

	std::cout << "\n" << "-------------------- " << std::endl;
	std::cout << "Average time = " << t / TESTS << std::endl;
	system("pause");
	return 0;
}