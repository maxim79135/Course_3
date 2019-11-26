import numpy as np

def Simplex(A, b, c):
    #sizes of basic and nonbasic vectors
    basicSize = A.shape[0] # number of constraints, m
    nonbasicSize = A.shape[1] - basicSize #n-m, number of variables
        
    cindx = [i for i in range(0, len(c))]
    bindx = cindx[nonbasicSize:]
    cbT = np.array(c[nonbasicSize:])

    #main loop
    while True:
        for i in range(cbT.size):
            print('x'+str(bindx[i] + 1), c[bindx[i]], b[i], A[i])
        z = 0
        for i in range(cbT.size):
            z += c[bindx[i]] * b[i]
        delta = np.array([z])
        for i in range(A.shape[1]):
            z = 0
            for j in range(cbT.size):
                z += A[j][i] * c[bindx[j]]
            z -= c[i]
            delta = np.append(delta, z)

        print('Zi-Ci', end = ' ')
        for i in delta:
            print(i, end = ' ')
        print()
        print()
        
        if np.min(delta) >= 0:
            print('System has solutions')
            c_x = np.zeros(A.shape[1])
            for i in range(cbT.size):
                c_x[bindx[i]] = b[i]
            print(c_x)
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

A = np.array([[2, 1, -2, 1, 1, 0, 0],
              [1, 1, 4, 0, 0, 1, 0],
              [1, -2, 2, 1, 0, 0, 1]
             ], dtype=float)
c = np.array([2, -1, 6, 1, 0, 0, 0], dtype=float)
b = np.array([24, 22, 0], dtype=float)

Simplex(A, b, c)