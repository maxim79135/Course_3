#include <iostream>
#include <cstdlib>
#include <cmath>
#include <ctime>
#include <vector>
#include "Map.h"
#include "BinTree.h"
#include "State.h"
#include <ctime>
#include <omp.h>

#define TESTS 5
using namespace std;


bool check (Map*);
void printMap(Map*);

//��������� �������� ����
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
		//������� ������ ������
		int zero = map->find(0);
		shift_pos = rand() % 4;
		switch (shift_pos){
		case 0:
			//���� ������ ���� �������
			if (zero / map->getCols() != 0) {
				map = map->shift(shift_pos);
				i++;
			}
			continue;
		case 1:
			//���� ������ ���� �������
			if (zero % map->getCols() != map->getCols() - 1) {
				map = map->shift(shift_pos);
				i++;
			}
			continue;
		case 2:
			//���� ����� ���� �������
			if (zero / map->getCols() != map->getLines() - 1) {
				map = map->shift(shift_pos);
				i++;
			}
			continue;
		case 3:
			//���� ����� ���� �������
			if (zero % map->getCols() != 0) {
				map = map->shift(shift_pos);
				i++;
			}
			continue;
		}
	}

	return map;
}

// ������� ��� ������ ����� �� �����
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
	//�������� ������ ������� �����������  OMP
std::vector<State*> aOMP(Map* map) {

		BinTree open = BinTree();
		BinTree close = BinTree(new State(map, NULL));

		State* min = close.min();

		for (; min->getCost() != 0; min = open.min(), close.add(min), open.del(min))
		{
			//����� ������ ������
			int zero = min->getMap()->find(0);
			State* states[4] = { 0, 0, 0, 0 };


#pragma omp parallel sections 
			{

#pragma omp section 
				{
					//�������� �������� ������
					if (zero / map->getCols() != 0) {
						State* s = new State(min->getMap()->shift(0), min);
						if ((open.find(s) == NULL) && (close.find(s) == NULL)) {
							//open.add(s);
							states[0] = s;
						}
					}
				}

#pragma omp section 
				{
					//�������� �������� ������
					if (zero % map->getCols() != map->getCols() - 1) {
						State* s = new State(min->getMap()->shift(1), min);
						if ((open.find(s) == NULL) && (close.find(s) == NULL)) {
							//open.add(s);
							states[1] = s;
						}
					}
				}

#pragma omp section 
				{
					//�������� �������� �����
					if (zero / map->getCols() != map->getLines() - 1) {
						State* s = new State(min->getMap()->shift(2), min);
						if ((open.find(s) == NULL) && (close.find(s) == NULL)) {
							//open.add(s);
							states[2] = s;
						}
					}
				}

#pragma omp section 
				{
					//�������� �������� �����
					if (zero % map->getCols() != 0) {
						State* s = new State(min->getMap()->shift(3), min);
						if ((close.find(s) == NULL) && (open.find(s) == NULL)) {
							//open.add(s);
							states[3] = s;
						}
					}
				}

			}

			//���������� ������ ������ ��� ��������
			for (int i = 0; i < 4; i++)
			{
				if (states[i]) {
					open.add(states[i]);
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
	double tSer = 0;
	double tOMP = 0;

	do {
		system("cls");
		cout << "Enter field sizes: " << endl;
		cin >> lines >> cols;
	} while (cols*lines <= 4);

	std::vector<State*> ans;

	for (int i = 0; i < TESTS; i++) {
		srand(i);
		map = generateMap(lines, cols);

		std::cout << "\n" << "Case #" << i + 1 << ": " << std::endl;
		printMap(map);

		clock_t time = clock();
		ans = aOMP(map);
		time = clock() - time;
		/*cout << "\n" << "Solution:";
		for (int k = ans.size() - 1; k > 0; k--) {
			printMap(ans[k]->getMap());
		}*/
		cout << "\n" << "Time of OMP A*= " << (double)time / CLOCKS_PER_SEC;
		printMap(ans[0]->getMap());
		tOMP += (double)time / CLOCKS_PER_SEC;

	}

	std::cout << "\n" << "-------------------- " << std::endl;
	std::cout << "Average time OMP = " << tOMP / TESTS << std::endl;
	system("pause");
	return 0;
}