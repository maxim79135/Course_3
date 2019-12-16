n, a, b = map(int, input().split())
arr = []
for i in range(n):
    arr.append(list(map(int, input().split())))

W = arr
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
if W[a - 1][b - 1] == 10000000:
    print(-1)
else:
    print(W[a - 1][b - 1])
