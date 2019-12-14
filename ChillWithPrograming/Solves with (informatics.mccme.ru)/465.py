n, m = map(int, input().split())

arr = []
tmp = [0] * n
for _ in range(n):
	arr.append(list(tmp))

for i in range(m):
	a, b = map(int, input().split())
	arr[a - 1][b - 1] = 1
	arr[b - 1][a - 1] = 1

for i in range(n):
	for j in range(n):
		print(arr[i][j], end=' ')
	print()
