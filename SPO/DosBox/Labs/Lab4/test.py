import itertools
string = input() 
lst = []
for i in range(1,len(string)+1):
    for j in list(itertools.combinations(string, i)):
        lst.append(''.join(j))
for v in lst:
    print(v)

