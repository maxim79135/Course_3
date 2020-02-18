import pprint
import timeit

pp = pprint.PrettyPrinter(indent=4)

def puzz_breadth_first(start,end):
    """
    Breadth First algorithm
    """
    front = [[start]]
    expanded = []
    expanded_nodes=0
    while front:
        i = 0
        for j in range(1, len(front)):    #minimum
            if len(front[i]) > len(front[j]):
                i = j
        path = front[i]         
        front = front[:i] + front[i+1:]
        endnode = path[-1]
        if endnode in expanded: continue
        for k in moves(endnode):
            if k in expanded: continue
            front.append(path + [k])
        expanded.append(endnode)
        expanded_nodes += 1
        if endnode == end: break
    print("Expanded nodes:",expanded_nodes)
    print("Solution:")
    pp.pprint(path)

def puzz_astar(start,end):
    """
    A* algorithm
    """
    front = [[heuristic_2(start), start]] #optional: heuristic_1
    expanded = []
    expanded_nodes=0
    while front:
        i = 0
        for j in range(1, len(front)):
            if front[i][0] > front[j][0]:
                i = j
        path = front[i]
        front = front[:i] + front[i+1:]
        endnode = path[-1]
        if endnode == end:
            break
        if endnode in expanded: continue
        for k in moves(endnode):
            if k in expanded: continue
            newpath = [path[0] + heuristic_2(k) - heuristic_2(endnode)] + path[1:] + [k] 
            front.append(newpath)
            expanded.append(endnode)
        expanded_nodes += 1
        print(expanded_nodes)
    print("Expanded nodes:", expanded_nodes)
    print("Solution:")
    pp.pprint(path)


def moves(mat): 
    """
    Returns a list of all possible moves
    """
    output = []  

    m = eval(mat)   
    i = 0
    while 0 not in m[i]: i += 1
    j = m[i].index(0); #blank space (zero)

    if i > 0:                                   
      m[i][j], m[i-1][j] = m[i-1][j], m[i][j];  #move up
      output.append(str(m))
      m[i][j], m[i-1][j] = m[i-1][j], m[i][j]; 
      
    if i < len(m) - 1:
      m[i][j], m[i+1][j] = m[i+1][j], m[i][j]   #move down
      output.append(str(m))
      m[i][j], m[i+1][j] = m[i+1][j], m[i][j]

    if j > 0:                                                      
      m[i][j], m[i][j-1] = m[i][j-1], m[i][j]   #move left
      output.append(str(m))
      m[i][j], m[i][j-1] = m[i][j-1], m[i][j]

    if j < len(m) - 1:
      m[i][j], m[i][j+1] = m[i][j+1], m[i][j]   #move right
      output.append(str(m))
      m[i][j], m[i][j+1] = m[i][j+1], m[i][j]

    return output

def heuristic_1(puzz):
    """
    Counts the number of misplaced tiles
    """ 
    misplaced = 0
    compare = 0
    m = eval(puzz)
    for i in range(len(m)):
        for j in range(len(m)):
            if m[i][j] != compare:
                misplaced += 1
            compare += 1
    return misplaced

def heuristic_2(puzz):
    """
    Manhattan distance
    """  
    distance = 0
    m = eval(puzz)          
    for i in range(len(m)):
        for j in range(len(m)):
            if m[i][j] == 0: continue
            distance += abs(i - (m[i][j]/len(m))) + abs(j -  (m[i][j]%len(m)));
    return distance

if __name__ == '__main__':
    #puzzle = str([[1, 2, 6, 3], [4, 9, 5, 7], [8, 13, 11, 15], [12, 14, 0, 10]])
    #end = str([[0, 1, 2, 3],[4, 5, 6, 7], [8, 9, 10, 11],[12, 13, 14, 15]])
    #puzzle = str([[1, 3, 4], [5, 0, 2], [7, 6, 8]])
    #end = str([[0, 1, 2], [3, 4, 5], [6, 7, 8]])
    puzzle = str([[11, 10, 21, 6, 24],
                [2, 23, 0, 1, 8],
                [7, 16, 14, 22, 19],
                [9, 20, 4, 17, 18],
                [12, 13, 15, 3, 5]])
    end = str([[0, 1, 2, 3, 4],
               [5, 6, 7, 8, 9],
               [10, 11, 12, 13, 14],
               [15, 16, 17, 18, 19],
               [20, 21, 22, 23, 24]])
    start = timeit.default_timer()
    puzz_astar(puzzle, end)
    #puzz_breadth_first(puzzle,end)
    stop = timeit.default_timer()
    print('Time: ', stop - start)
