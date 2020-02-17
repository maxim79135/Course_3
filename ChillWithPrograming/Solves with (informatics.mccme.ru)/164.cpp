#include <iostream>
#include <vector>

using namespace std;

bool* visited;
int** a;
int n, s;
vector <int> vertexes;

void DFS(int vertex) {
	visited[vertex] = true;
	
	vertexes.push_back(vertex);

	for (int j = 0; j < n; j++) {
		if (a[vertex][j] != 0) 
			if (!visited[j])
				DFS(j);
				vertexes.pop_back();
	}
	
	for (int i = 0; i < vertexes.size(); i++) {
		cout << vertexes[i] << " ";
	}
	vertexes.clear();
	cout << endl;
}

int main() {
	cin >> n >> s;

	a = new int*[n];
	for (int i = 0; i < n; i++)
		a[i] = new int[n];

	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++)
			cin >> a[i][j];
	
	visited = new bool[n];
	DFS(0);
}
