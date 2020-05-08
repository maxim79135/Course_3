#include <list>
#include <iostream>

using namespace std;

extern "C" int** add_matrix(int** matrix1, int** matrix2)
{
	int **answer = new int*[sizeof(matrix1)];
	for (int i = 0; i < sizeof(matrix1); i++)
		answer[i] = new int[sizeof(matrix1[0])];

	for (int i = 0; i < sizeof(matrix1); i++)
	{
		for (int j = 0; j < sizeof(matrix1[0]); j++)
		{
			answer[i][j] = matrix1[i][j] + matrix2[i][j];
		}
	}

	return answer;
}