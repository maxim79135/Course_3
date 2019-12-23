import collections

d = {}
while True:
    s = input().split()
    if s == []:
        break
    if d.get(s[0]) != None:
        d[s[0]] += int(s[1])
    else:
        d[s[0]] = int(s[1])

d = collections.OrderedDict(sorted(d.items()))
for key, value in d.items():
    print(key + ' ' + str(value))
