n = int(input())

arr = []
count = 0
for i in range(n):
	arr.append(list(map(int, input().split())))
	count += sum(arr[i])

for i in range(n):
	for j in range(i + 1, n):
		if arr[i][j] == 1:
			print('{0} {1}'.format(i + 1, j + 1))
