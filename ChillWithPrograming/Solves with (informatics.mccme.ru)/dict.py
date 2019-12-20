n = int(input())
d = {}
for _ in range(n):
    first, second = input().split()
    d.update({first:second, second:first})
v = input()
print(d.get(v))
