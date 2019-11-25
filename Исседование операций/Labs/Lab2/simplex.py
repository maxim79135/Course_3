import numpy as np
import pandas as pd
import math

def Simplex(A, b, c):
    #sizes of basic and nonbasic vectors
    basicSize = A.shape[0] # number of constraints, m
    nonbasicSize = A.shape[1] - basicSize #n-m, number of variables
        
    cindx = [i for i in range(0, len(c))]
    bindx = cindx[nonbasicSize:]
    cbT = np.array(c[nonbasicSize:])
    columns = ['C', 'B']
    for i in range(A.shape[1]):
        columns.append('x' + str(i + 1))

    #main loop
    while True:
        coef = np.zeros(b.size)
        index = np.zeros(b.size, dtype=int)
        for i in range(b.size):
            coef[i] = c[bindx[i]]
            index[i] = bindx[i] + 1
        z = 0
        for i in range(b.size):
            z += c[bindx[i]] * b[i]
        delta = np.array([z])
        for i in range(A.shape[1]):
            z = 0
            for j in range(b.size):
                z += A[j][i] * c[bindx[j]]
            z -= c[i]
            delta = np.append(delta, z)

        table = np.c_[coef, b, A]
        df = pd.DataFrame(table, index=index, columns=columns)
        print(df)
        print()
        print(delta)
        
        flag = 0
        for i in delta:
            if i >= 0:
                flag = 1
        if flag == 0:
            print('No solutions') 
            return
            
        if np.min(delta) >= 0:
            print('System has solutions')
            c_x = np.zeros(A.shape[1] + 1)
            for i in range(cbT.size):
                c_x[bindx[i]] = b[i]

            print('F=' + str(delta[0]))
            for i in c_x:
                print(i, end=' ')
            print()

            mods = np.zeros(A.shape[1] - b.size)
            for i in range(A.shape[1] - b.size):
                mods[i] = c_x[i] % 1
            
            if np.max(mods) != 0:
                print("Step")
                max_i = np.argmax(mods)

                for i in range(cbT.size):
                    if bindx[i] == max_i:
                        cur_i = i
                        break
                
                new_line = np.zeros(A.shape[1])
                for j in range(new_line.size):
                        if A[cur_i][j] % 1 != 0:
                            new_line[j] = - (A[cur_i][j] % 1)
                delta *= -1
                tmp_delta = np.array(delta)
                for i in range(new_line.size):
                    if new_line[i] != 0:
                        tmp_delta[i] /= new_line[i]
                    else:
                        tmp_delta[i] = 10000000000000
                tmp_delta[0] = 10000000000000

                min_j= np.argmin(tmp_delta)
                min_i = cbT.size
                bindx.append(min_j)
                b = np.append(b, mods[max_i])
                c = np.append(c, 0)
                columns.append('x' + str(A.shape[1] + 1))
                tmp_col = np.zeros(b.size)
                tmp_col[-1] = 1
                A = np.r_[A, [new_line]]
                A = np.c_[A, tmp_col]
                min = A[min_i][min_j]
                b[min_i] /= min
                A[min_i] /= min
                
                for i in range(cbT.size):
                    if i != min_i:
                        min = A[i][min_j]
                        A[i] -= min * A[min_i]
                        b[i] -= min * b[min_i]

                continue
            else:
                return
        
        min_j= np.argmin(delta)
        min_j -= 1
        min = []
        min = np.array(min)
        for i in range(cbT.size):
            if A[i][min_j] != 0 and b[i] / A[i][min_j] > 0:
                min = np.append(min, b[i] / A[i][min_j])
            else:
                min = np.append(min, 10000000000000)
        print()
        min_i = np.argmin(min)
        bindx[min_i] = min_j
        min = A[min_i][min_j]
        b[min_i] /= min
        A[min_i] /= min
        
        for i in range(cbT.size):
            if i != min_i:
                min = A[i][min_j]
                A[i] -= min * A[min_i]
                b[i] -= min * b[min_i]

A = np.array([[1, 2, 3, 1, 0, 0],
              [4, 3, 2, 0, 1, 0],
              [3, 1, 1, 0, 0, 1]
             ], dtype=float)
c = np.array([4, 5, 6, 0, 0, 0], dtype=float)
b = np.array([35, 45, 40], dtype=float)

Simplex(A, b, c)
