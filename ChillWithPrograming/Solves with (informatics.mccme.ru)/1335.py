n, m = map(int, input().split())
arr = [[0] * n] * n
print(arr)
for i in range(m):
    a, b, l = map(int, input().split())
    arr[a - 1][b - 1] = l

print(arr)

W = [[0] * n] * n
for i in range(n):
    for j in range(n):
        if arr[i][j] == 0 or arr[i][j] == -1:
            W[i][j] = 10000000
        else:
            W[i][j] = arr[i][j]

for k in range(n):
    for i in range(n):
        for j in range(n):
            W[i][j] = min(W[i][j], W[i][k] + W[k][j])

print(W)
