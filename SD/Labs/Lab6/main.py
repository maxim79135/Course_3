name = input()
name2 = input()
l = list(ord(name[i]) - ord(name2[i]) for i in range(len(name)))
for k in l:
    print(k, end = ' ')