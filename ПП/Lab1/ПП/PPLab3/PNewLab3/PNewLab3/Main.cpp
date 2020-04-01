#include <conio.h>
#include <iostream>
#include <cstdlib>
#include <cmath>
#include <ctime>
#include <vector>
#include "Map.h"
#include "binTree.h"
#include "state.h"
#include <ctime>
#include <thread>
#include <mutex>

#define TESTS 1000
using namespace std;

bool check(Map*);
void printMap(Map*);

//Генератор игрового поля
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
	srand(time(0));
	while (i <= len * 20) {
		//Находим пустую клетку
		int zero = map->find(0);
		shift_pos = rand() % 4;
		switch (shift_pos) {
		case 0:
			//Если свехру есть квадрат
			if (zero / map->getCols() != 0) {
				map = map->shift(shift_pos);
				i++;
			}
			continue;
		case 1:
			//Если справа есть квадрат
			if (zero % map->getCols() != map->getCols() - 1) {
				map = map->shift(shift_pos);
				i++;
			}
			continue;
		case 2:
			//Если снизу есть квадрат
			if (zero / map->getCols() != map->getLines() - 1) {
				map = map->shift(shift_pos);
				i++;
			}
			continue;
		case 3:
			//Если слева есть квадрат
			if (zero % map->getCols() != 0) {
				map = map->shift(shift_pos);
				i++;
			}
			continue;
		}
	}
	return map;
}

// Функция для вывода матрицы на экран
void printMap(Map* map) {
	cout << endl;
	for (int i = 0; i < map->lines; ++i) {
		for (int j = 0; j < map->cols; ++j) {
			cout << map->map[i*map->cols + j] << '\t';
		}
		cout << endl;
	}
	cout << endl;
}

/////////////////////////////////////////////////////////ПОСЛЕДОВАТЕЛЬНЫЙ АЛГОРИТМ/////////////////////////////////////////////////////////
//Функция поиска решения головоломки
vector<State*> a(Map* map) {
	//Создаем открытый и закрытый список для состояний
	BinTree open = BinTree(new State(map, NULL));
	BinTree close = BinTree();
	State* min = open.min();

	close.add(min);
	open.del(min);

	//Пока нет состояния в котором расстояние равно 0 (признак упорядоченности)
	for (; min->getCost() != 0; min = open.min(), close.add(min), open.del(min))
	{
		//Находим пустую клетку
		int zero = min->getMap()->find(0);
		//Если свехру есть квадрат
		if (zero / map->getCols() != 0) {
			State* s = new State(min->getMap()->shift(0), min);
			if ((open.find(s) == NULL) && (close.find(s) == NULL)) {
				open.add(s);
			}
		}

		//Если справа есть квадрат
		if (zero % map->getCols() != map->getCols() - 1) {
			State* s = new State(min->getMap()->shift(1), min);
			if ((open.find(s) == NULL) && (close.find(s) == NULL)) {
				open.add(s);
			}
		}

		//Если снизу есть квадрат
		if (zero / map->getCols() != map->getLines() - 1) {
			State* s = new State(min->getMap()->shift(2), min);
			if ((open.find(s) == NULL) && (close.find(s) == NULL)) {
				open.add(s);
			}
		}

		//Если слева есть квадрат
		if (zero % map->getCols() != 0) {
			State* s = new State(min->getMap()->shift(3), min);
			if ((close.find(s) == NULL) && (open.find(s) == NULL)) {
				open.add(s);
			}
		}

	}
	vector<State*> solution;

	State* s = min;
	do
	{
		solution.push_back(s);
		s = s->getParent();
	} while (s != NULL);
	return solution;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////ПАРАЛЛЕЛЬНЫЙ СТАРЫЙ/////////////////////////////////////////////////////////
void thread_func(State* min, BinTree* close, BinTree* open, int i, State** states) {
	State* s = new State(min->getMap()->shift(i), min);
	if ((open->find(s) == NULL) && (close->find(s) == NULL)) {
		states[i] = s;
	}
}

//Функция поиска решения головоломки
vector<State*> aPar1(Map* map) {
	//Создаем открытый и закрытый список для состояний
	BinTree* open = new BinTree(new State(map, NULL));
	BinTree* close = new BinTree();
	State* min = open->min();
	vector<thread> threads;

	close->add(min);
	open->del(min);

	//Пока нет состояния в котором расстояние равно 0 (признак упорядоченности)
	for (; min->getCost() != 0; min = open->min(), close->add(min), open->del(min))
	{
		//Находим пустую клетку
		int zero = min->getMap()->find(0);
		State* states[4] = { 0,0,0,0 };
		bool if_possible[4] = { false, false, false, false };

		if (zero / map->getCols() != 0) if_possible[0] = true;
		if (zero % map->getCols() != map->getCols() - 1) if_possible[1] = true;
		if (zero / map->getCols() != map->getLines() - 1) if_possible[2] = true;
		if (zero % map->getCols() != 0) if_possible[3] = true;

		for (int i = 0; i < 4; i++) {
			if (if_possible[i]) {
				threads.emplace_back(thread_func, min, close, open, i, states);
			}
		}

		for (auto &thread_ : threads) {
			thread_.join();
		}
		threads.clear();
		for (int i = 0; i < 4; i++)
		{
			if (states[i]) {
				open->add(states[i]);
			}
		}
	}
	vector<State*> solution;

	State* s = min;
	do
	{
		solution.push_back(s);
		s = s->getParent();
	} while (s != NULL);
	return solution;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



/////////////////////////////////////////////////////////ПАРАЛЛЕЛЬНЫЙ НОВЫЙ/////////////////////////////////////////////////////////

std::mutex flag_mutex;
bool flag_solution = false;

vector<State*> resultP2;
mutex resultP2_mutex;

vector<State*> thread_func2(Map* map, State* min, BinTree* close, BinTree* open) {
	vector<State*> lol;
	//Пока нет состояния в котором расстояние равно 0 (признак упорядоченности)
	for (; min->getCost() != 0; min = open->min(), close->add(min), open->del(min))
	{
		//Находим пустую клетку
		int zero = min->getMap()->find(0);
		State* tmp_states[4] = { 0, 0, 0, 0 };

#pragma omp parallel sections
		{
#pragma omp section
			{
				//Если свехру есть квадрат
				if (zero / map->getCols() != 0) {
					State* s = new State(min->getMap()->shift(0), min);
					if ((open->find(s) == NULL) && (close->find(s) == NULL)) {
						tmp_states[0] = s;
					}
				}
			}
#pragma omp section
			{
				//Если справа есть квадрат
				if (zero % map->getCols() != map->getCols() - 1) {
					State* s = new State(min->getMap()->shift(1), min);
					if ((open->find(s) == NULL) && (close->find(s) == NULL)) {
						tmp_states[1] = s;
					}
				}
			}
#pragma omp section
			{
				//Если снизу есть квадрат
				if (zero / map->getCols() != map->getLines() - 1) {
					State* s = new State(min->getMap()->shift(2), min);
					if ((open->find(s) == NULL) && (close->find(s) == NULL)) {
						tmp_states[2] = s;
					}
				}
			}
#pragma omp section
			{
				//Если слева есть квадрат
				if (zero % map->getCols() != 0) {
					State* s = new State(min->getMap()->shift(3), min);
					if ((close->find(s) == NULL) && (open->find(s) == NULL)) {
						tmp_states[3] = s;
					}
				}
			}
		}
		for (int i = 0; i < 4; i++) {
			if (tmp_states[i]) {
				open->add(tmp_states[i]);
			}
		}
		flag_mutex.lock();
		if (flag_solution == true)
		{
			flag_mutex.unlock();
			return lol;
		}
		flag_mutex.unlock();

	}
	flag_mutex.lock();
	flag_solution = true;
	flag_mutex.unlock();
	State* s = min;
	vector <State*> solution;

	do
	{
		solution.push_back(s);
		s = s->getParent();
	} while (s != NULL);
	
	resultP2_mutex.lock();
	resultP2 = solution;
	resultP2_mutex.unlock();

	return lol;
}

vector<State*> aPar2(Map* map) {
	//Создаем открытый и закрытый список для состояний
	BinTree* open = new BinTree();
	BinTree* close = new BinTree(new State(map, NULL));
	State* min = close->min();

	vector<thread> threads;
	vector<BinTree*> open_branch;
	vector<BinTree*> close_branch;

	//Находим пустую клетку
	int zero = min->getMap()->find(0);
	int index = 0;
	//Если свехру есть квадрат
	if (zero / map->getCols() != 0) {
		open_branch.emplace_back(new BinTree(new State(min->getMap()->shift(0), NULL)));
		close_branch.emplace_back(new BinTree());
	}

	//Если справа есть квадрат
	if (zero % map->getCols() != map->getCols() - 1) { 
		open_branch.emplace_back(new BinTree(new State(min->getMap()->shift(1), NULL)));
		close_branch.emplace_back(new BinTree());
	}

	//Если снизу есть квадрат
	if (zero / map->getCols() != map->getLines() - 1) {
		open_branch.emplace_back(new BinTree(new State(min->getMap()->shift(2), NULL)));
		close_branch.emplace_back(new BinTree());
	}

	//Если слева есть квадрат
	if (zero % map->getCols() != 0) {
		open_branch.emplace_back(new BinTree(new State(min->getMap()->shift(3), NULL)));
		close_branch.emplace_back(new BinTree());
	}

	for (int i = 0; abs(i) < 1; i++) {
		threads.emplace_back(thread_func2, open_branch[i]->min()->getMap(), open_branch[i]->min(), close_branch[i], open_branch[i]);
	}

	for (auto &thread_ : threads) {
		thread_.join();
	}

	return resultP2;
	
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


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
			cout << "The size of the field is wrong" << endl;
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			_getch();
		}
	} while (cols*lines <= 4);

	double tSer = 0;
	double tParOld = 0;
	double tParNew = 0;
	vector<State*> ans;

	for (int i = 0; i < TESTS; i++) {
		srand(i);
		map = generateMap(lines, cols);

		cout << "\n" << "-----------------------------------------------------";
		cout << "\n" << "Case #" << i + 1 << ": ";
		printMap(map);

		//Решение последовательным
		clock_t time = clock();
		//ans = a(map);
		//time = clock() - time;
		///*cout << "\n" << "Solution:";
		//for (int k = ans.size() - 1; k > 0; k--) {
		//	printMap(ans[k]->getMap());
		//}*/ 
		//cout << "\n" << "Time of SERIAL = " << (double)time / 1000;
		//printMap(ans[0]->getMap());
		//tSer += (double)time / 1000;

		////Решение старым параллельным
		//time = clock();
		//ans = aPar1(map);
		//time = clock() - time;
		///*cout << "\n" << "Solution:";
		//for (int k = ans.size() - 1; k > 0; k--) {
		//	printMap(ans[k]->getMap());
		//}*/
		//cout << "\n" << "Time of OLD PARALLEL= " << (double)time / 1000;
		//printMap(ans[0]->getMap());
		//tParOld += (double)time / 1000;

		//Решение новым параллельным
		time = clock();
		ans = aPar2(map);
		time = clock() - time;
		/*cout << "\n" << "Solution:";
		for (int k = ans.size() - 1; k > 0; k--) {
			printMap(ans[k]->getMap());
		}*/
		cout << "\n" << "Time of NEW PARALLEL= " << (double)time / 1000;
		printMap(ans[0]->getMap());
		tParNew += (double)time / 1000;
	}
	//Начиная с 4 на 4 можно старый параллельный не считать, 1000 выборок
	cout << "\n" << "--------------" << endl;
	cout << "Average time SERIAL = " << tSer / TESTS << endl;
	cout << "Average time OLD PARALLEL = " << tParOld / TESTS << endl;
	cout << "Average time NEW PARALLEL = " << tParNew / TESTS << endl;
	cout << "--------------" << endl;

	system("pause");
	return 0;
}