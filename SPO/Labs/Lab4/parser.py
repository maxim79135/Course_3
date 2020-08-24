import itertools
f = open('input.txt', 'r')
buf = f.readline()
string = buf[:-1]
lst = []
for i in range(1,len(string)+1):
    for j in list(itertools.combinations(string, i)):
        lst.append(''.join(j))

l = []
buf = f.readline()
signals = [i for i in buf if i != '\n']

while True:
    line = f.readline()
    if not line:
        break
    line = line.split(',')
    l.append((line[0], line[1], line[2][0]))
    for v in l:
        print('F({0},{1}={2}'.format(v[0],v[1],v[2]))
        
g = open('output.txt', 'w')
for value in lst:
    for signal in signals:
        s = []
        for symbol in value:
            for correct_line in l:
                if symbol == correct_line[0] and signal == correct_line[1]:
                    if not correct_line[2] in s:
                        s.append(correct_line[2])
        buf = ''
        for v in s:
            buf += v
        if buf == '':
            buf = '-'
        #print('F({0}, {1}) = {2}'.format(value, signal, buf))
        g.write('F({0}, {1}) = {2}\n'.format(value, signal, buf))
