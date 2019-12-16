import numpy as np
from scipy.optimize import linear_sum_assignment

def main(cost, mode):
    tmp = np.array(cost)
    if mode == 'max':
        for i in range(cost.shape[0]):
            cost[i] = np.max(cost[i]) - cost[i]
    print(cost)
    row_ind, col_ind = linear_sum_assignment(cost)
    return tmp[row_ind, col_ind].sum()

if __name__ == "__main__":
    f = open('input.txt', 'r')
    s = f.readline().split()
    n = int(s[0])
    mode = s[1]
    table = np.zeros((n, n), dtype=int)

    for i in range(n):
        table[i] = list(map(int, f.readline().split()))
    result = main(table, mode)
    print(result)
