import random
from sys import argv

f = open('input.txt', 'w')
count = int(input())
numbers = 0
ar = []
for _ in range(count):
    ar.append(list([]))
expanded = []
while numbers != count * count:
    while True:
        num = random.randint(0, count * count - 1)
        if num not in expanded:
            break
    expanded.append(num)
    ar[numbers // count].append(num)
    numbers += 1

for i in range(len(ar)):
    for j in range(len(ar[0])):
        f.write(str(ar[i][j]) + ' ')
    f.write('\n')
