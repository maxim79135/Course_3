n = int(input())

arr = []
for i in range(n):
	arr.append(list(map(int, input().split())))

for i in range(n):
	for j in range(n):
		if arr[i][j] == 1:
			print('{0} {1}'.format(i + 1, j + 1))
