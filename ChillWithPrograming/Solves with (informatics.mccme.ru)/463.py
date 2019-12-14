n = int(input())

arr = []
count = 0
for i in range(n):
	arr.append(list(map(int, input().split())))
	count += sum(arr[i])

print(count)
