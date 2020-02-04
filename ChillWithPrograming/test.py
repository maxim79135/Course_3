n, k = map(int, input().split())
l = list(map(int, input().split()))
print(l.sort()[k - 1])
