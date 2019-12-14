"""
sadasdas

0 1 1 0
0 0 1 0
0 0 0 0
0 1 0 0
"""

n = int(input())
out = []
for _ in range(n):
	out.append(list([]))
for i in range(n):
	nodes = map(int, input().split())
	for node in nodes:
		out[node - 1].append(i + 1)
print(n)
for nodes in out:
	print(*nodes,sep=' ')
