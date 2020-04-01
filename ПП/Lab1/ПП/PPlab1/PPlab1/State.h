#pragma once
#include <cstdlib>
#include "map.h"

int costFunc(Map* map);
unsigned int calcHash(Map* map);

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

int costFunc(Map* map) {
	int len = map->lines*map->cols;
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


unsigned int calcHash(Map* map) {
	int len = map->lines * map->cols;
	unsigned int h = 0;
	for (int i = 0; i < len; ++i) {
		h += std::hash<int>{}(map->map[i] * (1 << i));
	}

	return h;
}
