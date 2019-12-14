def main():
	n, m = map(int, input().split())
	arr = []
	tmp = [0] * n
	for _ in range(n):
		arr.append(list(tmp))

	for i in range(m):
		a, b = map(int, input().split())
		if arr[a - 1][b - 1] == 1:
			print('YES')
			return
		arr[a - 1][b - 1] = 1
	print('NO')


main()
