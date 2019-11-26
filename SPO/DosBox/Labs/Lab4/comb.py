import itertools

lst = []
string = input()
for i in range(1, len(string) + 1):
    for j in list(itertools.combinations(string, i)):
        lst.append("".join(j))
print(lst)  