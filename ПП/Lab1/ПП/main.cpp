#include <mpi.h>
#include <conio.h>
#include <iostream>
#include <cmath>
#include <vector>
#include "Map.h"
#include "BinTree.h"
#include "State.h"
#include <chrono>
#include <mutex>
using namespace std;

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
	while (i <= len*3.5) {
		//Находим пустую клетку (3,5 > 3x3, 20 <= 3x3)
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
			cout << map->map[i * map->cols + j] << '\t';
		}
		cout << endl;
	}
	cout << endl;
}


/////////////////////////////////////////////////////////ПАРАЛЛЕЛЬНЫЙ НОВЫЙ/////////////////////////////////////////////////////////

vector<State*> resultP2;

vector<State*> thread_func2(Map* map, State* min, BinTree* close, BinTree* open, int flag_active_arr[4], int rank) {
	vector<State*> lol;

	int flag_solution[4] = { 0, 0, 0, 0 };
	MPI_Request request[4];

	MPI_Request request0;
	MPI_Request request1;
	MPI_Request request2;
	MPI_Request request3;

	int flag_request[4] = { 0, 0, 0, 0 };
	MPI_Status status0;
	MPI_Status status1;
	MPI_Status status2;
	MPI_Status status3;

	if (flag_active_arr[0] == 1)
		MPI_Irecv(&flag_solution[0], 1, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &request0);
	if (flag_active_arr[1] == 1)
		MPI_Irecv(&flag_solution[1], 1, MPI_INT, 1, MPI_ANY_TAG, MPI_COMM_WORLD, &request1);
	if (flag_active_arr[2] == 1)
		MPI_Irecv(&flag_solution[2], 1, MPI_INT, 2, MPI_ANY_TAG, MPI_COMM_WORLD, &request2);
	if (flag_active_arr[3] == 1)
		MPI_Irecv(&flag_solution[3], 1, MPI_INT, 3, MPI_ANY_TAG, MPI_COMM_WORLD, &request3);
	//Пока нет состояния в котором расстояние равно 0 (признак упорядоченности)
	while ((flag_request[0] == 0) && (flag_request[1] == 0) && (flag_request[2] == 0) && (flag_request[3] == 0))
	{
		if (flag_active_arr[0] == 1)
			MPI_Test(&request0, &flag_request[0], &status0);
		if (flag_active_arr[1] == 1)
			MPI_Test(&request1, &flag_request[1], &status1);
		if (flag_active_arr[2] == 1)
			MPI_Test(&request2, &flag_request[2], &status2);
		if (flag_active_arr[3] == 1)
			MPI_Test(&request3, &flag_request[3], &status3);
		if ((flag_request[0] == 0) && (flag_request[1] == 0) && (flag_request[2] == 0) && (flag_request[3] == 0))
		{
			min = open->min();
			close->add(min);
			open->del(min);

			//Находим пустую клетку
			int zero = min->getMap()->find(0);
			//Если сверху есть квадрат
			if (zero / map->getCols() != 0) {
				State* s = new State(min->getMap()->shift(0), min);
				if ((open->find(s) == NULL) && (close->find(s) == NULL)) {
					open->add(s);
				}
			}

			//Если справа есть квадрат
			if (zero % map->getCols() != map->getCols() - 1) {
				State* s = new State(min->getMap()->shift(1), min);
				if ((open->find(s) == NULL) && (close->find(s) == NULL)) {
					open->add(s);
				}
			}

			//Если снизу есть квадрат
			if (zero / map->getCols() != map->getLines() - 1) {
				State* s = new State(min->getMap()->shift(2), min);
				if ((open->find(s) == NULL) && (close->find(s) == NULL)) {
					open->add(s);
				}
			}

			//Если слева есть квадрат
			if (zero % map->getCols() != 0) {
				State* s = new State(min->getMap()->shift(3), min);
				if ((close->find(s) == NULL) && (open->find(s) == NULL)) {
					open->add(s);
				}
			}
			if (min->getCost() == 0)
			{
				flag_solution[rank] = 1;
				if (flag_active_arr[0] == 1)
					MPI_Isend(&flag_solution[rank], 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &request0);
				if (flag_active_arr[1] == 1)
					MPI_Isend(&flag_solution[rank], 1, MPI_INT, 1, 1, MPI_COMM_WORLD, &request1);
				if (flag_active_arr[2] == 1)
					MPI_Isend(&flag_solution[rank], 1, MPI_INT, 2, 2, MPI_COMM_WORLD, &request2);
				if (flag_active_arr[3] == 1)
					MPI_Isend(&flag_solution[rank], 1, MPI_INT, 3, 3, MPI_COMM_WORLD, &request3);
				flag_solution[rank] = 1;
			}

		}
	}
	return lol;
}

vector<State*> aPar2(Map* map, int rank) {

	int flag_active_arr[4] = { 0,0,0,0 };
	//Создаем открытый и закрытый список для состояний
	BinTree* open = NULL;
	BinTree* close = NULL;

	BinTree* closekek = new BinTree(new State(map, NULL));
	State* min = closekek->min();

	//Находим пустую клетку
	int zero = min->getMap()->find(0);
	int index = 0;
	//Если сверху есть квадрат
	if (zero / map->getCols() != 0) {
		flag_active_arr[0] = 1;
		if (rank == 0)
		{
			open = new BinTree(new State(min->getMap()->shift(0), NULL));
			close = new BinTree();
		}
	}

	//Если справа есть квадрат
	if (zero % map->getCols() != map->getCols() - 1) {
		flag_active_arr[1] = 1;
		if (rank == 1)
		{
			open = new BinTree(new State(min->getMap()->shift(1), NULL));
			close = new BinTree();
		}
	}

	//Если снизу есть квадрат
	if (zero / map->getCols() != map->getLines() - 1) {
		flag_active_arr[2] = 1;
		if (rank == 2)
		{
			open = new BinTree(new State(min->getMap()->shift(2), NULL));
			close = new BinTree();
		}
	}

	//Если слева есть квадрат
	if (zero % map->getCols() != 0) {
		flag_active_arr[3] = 1;
		if (rank == 3)
		{
			open = new BinTree(new State(min->getMap()->shift(3), NULL));
			close = new BinTree();
		}
	}

	//MPI_Allgather(&flag_active_arr[rank], 1, MPI_INT, flag_active_arr, 1, MPI_INT, MPI_COMM_WORLD);

	if (flag_active_arr[rank] == true)
	{
		resultP2 = thread_func2(open->min()->getMap(), open->min(), close, open, flag_active_arr, rank);
	}
	//else
		//resultP2 = thread_func2(NULL, NULL, NULL, NULL, flag_active_arr, rank);
	return resultP2;

}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int main(int argc, char** argv)
{
	int rank, size;
	int lines = 0;
	int cols = 0;
	Map* map;
	vector<State*> ans;
	long double tParNew = 0;

	MPI_Init(&argc, &argv);

	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

		do {
			if (rank == 0) 
			{
				system("cls");
				cout << "Enter field sizes: " << endl;
				cin >> lines >> cols;
			}
			MPI_Barrier(MPI_COMM_WORLD);
			MPI_Bcast(&lines, 1, MPI_INT, 0, MPI_COMM_WORLD);
			MPI_Bcast(&cols, 1, MPI_INT, 0, MPI_COMM_WORLD);
			if (rank == 0)
			{
				cin.clear();
				if (cin.good() == false || cols < 2 || lines < 2) {
					system("cls");
					cout << "The size of the field is wrong" << endl;
					cin.ignore(numeric_limits<streamsize>::max(), '\n');
					_getch();
				}
			}
		} while (cols * lines <= 4);

	int i = 0;
	for (i; i < TESTS; i++) {
		srand(i);
		map = generateMap(lines, cols);
		if (rank == 0)
		{
			cout << "\n" << "-----------------------------------------------------";
			cout << "\n" << "Case #" << i + 1 << ": ";
			printMap(map);
		}
		//Решение новым параллельным
		//MPI_Barrier(MPI_COMM_WORLD);
		//MPI_Bcast(&(map->lines), 1, MPI_INT, 0, MPI_COMM_WORLD);
		//MPI_Bcast(&(map->cols), 1, MPI_INT, 0, MPI_COMM_WORLD);
		MPI_Bcast(map->map, lines * cols, MPI_INT, 0, MPI_COMM_WORLD);

		chrono::high_resolution_clock::time_point t11 = chrono::high_resolution_clock::now();
		ans = aPar2(map, rank);
		chrono::high_resolution_clock::time_point t22 = chrono::high_resolution_clock::now();
		MPI_Barrier(MPI_COMM_WORLD);
		long double duration2 = chrono::duration_cast<chrono::milliseconds>(t22 - t11).count();
		if (rank == 0)
			cout << "\n" << "Time of NEW PARALLEL = " << duration2;
		tParNew += duration2;
	}
	if (rank == 0)
	{
		cout << "\n" << "--------------" << endl;
		cout << "Average time NEW PARALLEL = " << (tParNew / (double)TESTS) << endl;
		cout << "--------------" << endl;
		system("pause");
	}
	MPI_Barrier(MPI_COMM_WORLD);
	MPI_Finalize();
	return 0;
}