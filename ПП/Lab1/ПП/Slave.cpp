#include <curses.h>
#include <iostream>
#include <cmath>
#include <vector>
#include <chrono>
#include <pvm3.h> 

class Map
{
public:
	int* map;
	int lines;
	int cols;
public:
	Map(int lines, int cols) {
		this->map = new int[lines * cols];
		this->lines = lines;
		this->cols = cols;
	}

	int getLines() {
		return this->lines;
	}

	int getCols() {
		return this->cols;
	}

	int find(int n) {
		for (int i = 0; i < lines * cols; ++i) {
			if (map[i] == n) return i;
		}
		return  -1;
	}

	Map* shift(int angle) {
		if ((angle < 0) || (angle > 3)) return this;

		int len = lines * cols;
		Map * mapc = new Map(lines, cols);

		int ind = 0;

		for (int i = 0; i < len; ++i) {
			mapc->map[i] = map[i];
			if (map[i] == 0) ind = i;
		}

		switch (angle) {
			//Вверх
		case 0: {
			mapc->map[ind] = mapc->map[ind - cols];
			mapc->map[ind - cols] = 0;
			break;
		}
				//Вправо
		case 1: {
			mapc->map[ind] = mapc->map[ind + 1];
			mapc->map[ind + 1] = 0;
			break;
		}
				//Вниз	
		case 2: {
			mapc->map[ind] = mapc->map[ind + cols];
			mapc->map[ind + cols] = 0;
			break;
		}
				//Влево
		case 3: {
			mapc->map[ind] = mapc->map[ind - 1];
			mapc->map[ind - 1] = 0;
			break;
		}
		}
		return mapc;
	}
};


int costFunc(Map * map);
unsigned int calcHash(Map * map);

class State
{
public:
	int cost;
	State* parent;
	Map* map;
	int hash;
public:
	State(Map* map, State* parent) {
		this->cost = costFunc(map);
		this->parent = parent;
		this->map = map;
		this->hash = calcHash(map);
	}

	State() {}

	const State* copy() const {
		return this;
	}

	Map* getMap() const {
		return this->map;
	}

	int getHash() const {
		return this->hash;
	}

	int getCost() const {
		return this->cost;
	}

	State* getParent() {
		return this->parent;
	}
};

int costFunc(Map * map) {
	int len = map->lines * map->cols;
	int sum = 0;
	for (int i = 0; i < len; ++i)
	{
		if (map->map[i] == 0) continue;
		int dx = abs((i % map->cols) - ((map->map[i] - 1) % map->cols));
		int dy = abs((i / map->cols) - ((map->map[i] - 1) / map->cols));
		sum += dx + dy;
	}

	return sum;
}


unsigned int calcHash(Map * map) {
	int len = map->lines * map->cols;
	unsigned int h = 0;
	for (int i = 0; i < len; ++i) {
		h += std::hash<int>{}(map->map[i] * (1 << i));
	}

	return h;
}


struct Node {
	Node* left;
	Node* right;
	State* elem;
};

class BinTree
{
private:
	Node* node;
public:
	int len;

	BinTree(State* s) {
		this->node = new Node();
		this->node->elem = s;
		len = 1;
	}

	BinTree() {
		this->node = new Node();
		len = 0;
	}

	~BinTree() {
	}

	int getLen() {
		return len;
	}

	void add(State* s) {
		Node* n = this->node;

		len += 1;

		if (n->elem == NULL) {
			n->elem = s;
			return;
		}
		do
		{
			if (s->getCost() >= n->elem->getCost()) {
				if (n->right == NULL) {
					n->right = new Node();
					n->right->elem = s;
					break;
				}
				else {
					n = n->right;
				}
			}
			else {
				if (n->left == NULL) {
					n->left = new Node();
					n->left->elem = s;
					break;
				}
				else {
					n = n->left;
				}
			}

		} while (true);
	}

	void del(State* s) {
		Node* n = this->node;
		Node* p = NULL;

		while (n->elem != s)
		{
			p = n;
			if (s->getCost() > n->elem->getCost()) {
				n = n->right;
			}
			else {
				n = n->left;
			}

			if (n == NULL) return;
		}

		len -= 1;

		if ((n->left == NULL) && (n->right == NULL)) {
			if (p == NULL) {
				n->elem = NULL;
				return;
			}
			if (p->left == n) p->left = NULL;
			else p->right = NULL;
			delete n;
			return;
		}

		if (n->left == NULL) {
			if (p == NULL) {
				this->node = n->right;
				delete n;
				return;
			}
			if (p->left == n) p->left = n->right;
			else p->right = n->right;
			delete n;
			return;
		}

		if (n->right == NULL) {
			if (p == NULL) {
				this->node = n->left;
				delete n;
				return;
			}
			if (p->left == n) p->left = n->left;
			else p->right = n->left;
			delete n;
			return;
		}

		if (n->right->left == NULL) {
			n->elem = n->right->elem;
			n->right = n->right->right;
			return;
		}
		else {
			Node* k = n->right;

			while (k->left->left != NULL) {
				k = k->left;
			}

			n->elem = k->left->elem;
			k->left = k->left->right;
		}

	}
	//Компаратор при коллизии хешей
	bool cmp(State * a, State * b) {
		Map* am = a->getMap();
		Map* bm = b->getMap();
		int len = am->getCols() * am->getLines();
		for (int i = 0; i < len; i++) {
			if (am->map[i] != bm->map[i]) return false;
		}
		return true;

	}
	//Поиск элемента в дереве
	State* find(State * s) {
		Node* n = this->node;

		if (n->elem == NULL) return NULL;

		while ((n->elem->getHash() != s->getHash()) || (!cmp(n->elem, s))) {
			if (s->getCost() >= n->elem->getCost()) {
				n = n->right;
			}
			else {
				n = n->left;
			}

			if (n == NULL) return NULL;
		}
		return n->elem;
	}
	//Поиск минимального элемента дерева
	State* min() {
		Node* n = this->node;

		while (n->left != NULL) n = n->left;

		return n->elem;
	}
};

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
	while (i <= len * 3.5) {
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
void printMap(Map * map) {
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

#define FINALTAG 7
vector<State*> resultP2;

vector<State*> thread_func2(Map * map, State * min, BinTree * close, BinTree * open, int child[4], int myparent) {
	vector<State*> lol;
	int flag_solution = 0;
	int local_flag_solution = 0;
	int child_with_parent[5] = { 0,0,0,0,0 };
	child_with_parent[0] = myparent;
	child_with_parent[1] = child[0];
	child_with_parent[2] = child[1];
	child_with_parent[3] = child[2];
	child_with_parent[4] = child[3];



	//Пока нет состояния в котором расстояние равно 0 (признак упорядоченности)
	while ((pvm_nrecv(-1, FINALTAG) == 0) || (local_flag_solution == 0))
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
				flag_solution = 1;
				local_flag_solution = 1;
				int  bufid = pvm_initsend(PvmDataDefault);
				int  info = pvm_pkint(&flag_solution, 1, 1);
				info = pvm_mcast(child_with_parent, 5, FINALTAG);
				break;
			}
	}
	return lol;
}
vector<State*> aPar2(Map * map, int side, int child[4], int myparent) {

	int flag_active = 0;
	//Создаем открытый и закрытый список для состояний
	BinTree* open = NULL;
	BinTree* close = NULL;

	BinTree* closekek = new BinTree(new State(map, NULL));
	State* min = closekek->min();

	//Находим пустую клетку
	int zero = min->getMap()->find(0);
	int index = 0;
	switch (side) { 
	case 0:
		//Если сверху есть квадрат
		if (zero / map->getCols() != 0) {
			flag_active = 1;
			open = new BinTree(new State(min->getMap()->shift(0), NULL));
			close = new BinTree();
		}
		break;
	case 1:
		//Если справа есть квадрат
		if (zero % map->getCols() != map->getCols() - 1) {
			flag_active = 1;
			open = new BinTree(new State(min->getMap()->shift(1), NULL));
			close = new BinTree();
		}
		break;
	case 2:
		//Если снизу есть квадрат
		if (zero / map->getCols() != map->getLines() - 1) {
			flag_active = 1;
			open = new BinTree(new State(min->getMap()->shift(2), NULL));
			close = new BinTree();
		}
		break;
	case 3:
		//Если слева есть квадрат
		if (zero % map->getCols() != 0) {
			flag_active = 1;
			open = new BinTree(new State(min->getMap()->shift(3), NULL));
			close = new BinTree();
		}
		break;
	}


	//MPI_Allgather(&flag_active_arr[rank], 1, MPI_INT, flag_active_arr, 1, MPI_INT, MPI_COMM_WORLD);

	if (flag_active == 1)
	{
		resultP2 = thread_func2(open->min()->getMap(), open->min(), close, open, child, myparent);
	}
	//else
		//resultP2 = thread_func2(NULL, NULL, NULL, NULL, flag_active_arr, rank);
	return resultP2;

}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define  SIZETAG 5       /* идентификатор для других сообщений */
#define MAPTAG 6
#define CHILDTAG 8

int main(int argc, char** argv)
{
	int rank, size;
	int lines = atoi(argv[2]);
	int cols = atoi(argv[3]);
	int TESTS = atoi(argv[4]);

	int side = -1;
	Map* map;
	vector<State*> ans;
	long double tParNew = 0;
	int  child[4];  /* массив идентификаторов дочерних задач */
	
	int mytid = pvm_mytid();   /* определить собственный идентификатор задачи */
	int myparent = pvm_parent();    /* узнать идентификатор родительской задачи */
		
	pvm_recv(     /* получить от родительской задачи значения m и blksize. */
		myparent,  /* от кого : идентификатор задачи, которая */							   /* в группе MY_GROUP имеет номер 0. */
		CHILDTAG /* идентификатор ожидаемого сообщения */
	);
	pvm_upkint(child, 4, 1);       /* распаковать принятые значения m */
	
	map = new Map(lines, cols);
	
	if (mytid == child[0]) 
		side = 0;
	else
	if (mytid == child[1]) 
		side = 1;
	else
	if (mytid == child[2]) 
		side = 2;
	else
	if (mytid == child[3]) 
		side = 3;
	
	for (int i = 0; i < TESTS; i++) {
		pvm_recv(     /* получить от родительской задачи значения m и blksize. */
			myparent,  /* от кого : идентификатор задачи, которая */
									   /* в группе MY_GROUP имеет номер 0. */
			MAPTAG                     /* идентификатор ожидаемого сообщения */
		);
		pvm_upkint(map->map, lines * cols, 1);       /* распаковать принятые значения m */
		ans = aPar2(map, side, child, myparent);
	}
	pvm_exit();    /* отключиться от PVM и завершить работу дочерней задачи */
	return 0;
}