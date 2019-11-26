import numpy as np
import pandas as pd

def print_matrix(table, needs, stocks):
    index = ['A' + str(i + 1) for i in range(table.shape[0])]
    index.append('Needs')
    columns = ['B' + str(i + 1) for i in range(table.shape[1])]
    columns.append('Stocks')

    temp_table = table
    temp_table = np.r_[temp_table, [needs]]
    temp_table = np.c_[temp_table, np.append(stocks, np.sum(stocks))]
    
    df = pd.DataFrame(temp_table, index=index, columns=columns)
    print(df)
    print()

max_value = 10000000

def reference_plan_calculation(table, needs, stocks):
    temp_table = np.array(table)
    temp_needs = np.array(needs)
    temp_stocks = np.array(stocks)
    temp_values = np.full((table.shape[0], table.shape[1]), -1, dtype=int)
    count = 0

    while np.min(temp_table) != max_value:
        cur_i, cur_j = np.argmin(temp_table) // table.shape[1], np.argmin(temp_table) % table.shape[1]
        if np.argmin([temp_needs[cur_j], temp_stocks[cur_i]]) == 0:
            temp_values[cur_i][cur_j] = temp_needs[cur_j]
            temp_stocks[cur_i] -= temp_needs[cur_j]
            if temp_stocks[cur_i] == 0:
                for j in range(table.shape[1]):
                    temp_table[cur_i][j] = max_value
            temp_needs[cur_j] = 0
            for i in range(table.shape[0]):
                temp_table[i][cur_j] = max_value
        else:
            temp_values[cur_i][cur_j] = temp_stocks[cur_i]
            temp_needs[cur_j] -= temp_stocks[cur_i]
            if temp_needs[cur_j] == 0:
                for i in range(table.shape[0]):
                    temp_table[i][cur_j] = max_value
            temp_stocks[cur_i] = 0
            for j in range(table.shape[1]):
                temp_table[cur_i][j] = max_value
        print_matrix(temp_values, temp_needs, temp_stocks)
        count += 1
    
    flag = 0
    if count < table.shape[0] + table.shape[1] - 1:
        for i in range(table.shape[0]):
            for j in range(table.shape[1]):
                if temp_values[i][j] == -1:
                    temp_values[i][j] = 0
                    flag = 1
                    break
            if flag == 1:
                break
    print_matrix(temp_values, temp_needs, temp_stocks)                    
    return temp_values

def get_delta(plan, table):
    #init potential
    for i in range(table.shape[0]):
        for j in range(table.shape[1]):
            if plan[i][j] != -1:
                print('u' + str(i + 1) + ' + ' + 'v' + str(j + 1) + ' = ' + str(table[i][j]))

    u = np.full(table.shape[0], max_value)
    v = np.full(table.shape[1], max_value)

    u[0] = 0
    count = 0
    list_u, list_v = [], []
    list_u.append(0)
    
    while count != table.shape[0] + table.shape[1] - 1:
        while len(list_u) != 0:
            cur_i = list_u.pop(0)
            for j in range(table.shape[1]):
                if plan[cur_i][j] != -1:
                    if v[j] == max_value:
                        v[j] = table[cur_i][j] - u[cur_i]
                        list_v.append(j)
                        count += 1
        
        while len(list_v) != 0:
            cur_j = list_v.pop(0)
            for i in range(table.shape[0]):
                if plan[i][cur_j] != -1:
                    if u[i] == max_value:
                        u[i] = table[i][cur_j] - v[cur_j]
                        list_u.append(i)
                        count += 1
    print()
    for i in range(len(u)):
        print('u' + str(i + 1) + ' = ' + str(u[i]))
    print()
    for i in range(len(v)):
        print('v' + str(i + 1) + ' = ' + str(v[i]))

    #solve delta
    print('Delta: ', end=' ')
    min_delta_i, min_delta_j, min_delta = max_value, max_value, max_value
    for i in range(table.shape[0]):
        for j in range(table.shape[1]):
            if plan[i][j] == -1:
                if table[i][j] - (u[i] + v[j]) < min_delta:
                    min_delta = table[i][j] - (u[i] + v[j])
                    min_delta_i = i
                    min_delta_j = j
                print(table[i][j] - (u[i] + v[j]), end=' ')
    print()
    return min_delta, min_delta_i, min_delta_j

def get_way(plan, min_delta_i, min_delta_j):
    pass

Сед789836#
CxP7EvD
def change_plan(plan, min_delta_i, min_delta_j):
    nodes = [(min_delta_i, min_delta_j), (1, 2), (2, 2), (2, 1)]
    #get_way(plan, min_delta_i, min_delta_j)

    min = max_value
    for i in range(len(nodes)):
        if plan[nodes[i][0]][nodes[i][1]] < min and plan[nodes[i][0]][nodes[i][1]] != -1 and i % 2 != 0:
            min = plan[nodes[i][0]][nodes[i][1]]
    
    for i in range(len(nodes)):
        if i % 2 == 0:
            if plan[nodes[i][0]][nodes[i][1]] == -1:
                plan[nodes[i][0]][nodes[i][1]] = min
            else:
                plan[nodes[i][0]][nodes[i][1]] += min
        else:
            plan[nodes[i][0]][nodes[i][1]] -= min
            if plan[nodes[i][0]][nodes[i][1]] == 0:
                plan[nodes[i][0]][nodes[i][1]] = -1
    return

def main():
    print('Enter option: ', end='')
    option = input()
    f = open('input_' + option + '.txt', 'r')
    count_providers, count_consumers = map(int, f.readline().split())
    table = np.zeros((count_providers, count_consumers), dtype=int)

    for i in range(count_providers):
        table[i] = list(map(int, f.readline().split()))

    needs = np.array(list(map(int, f.readline().split())))
    stocks = np.array(list(map(int, f.readline().split())))
    
    # change table for closed model
    if np.sum(needs) < np.sum(stocks):
        table = np.c_[table, np.zeros(table.shape[0])]
        needs = np.append(needs, np.sum(stocks) - np.sum(needs))
    elif np.sum(needs) > np.sum(stocks):
        table = np.r_[table, [np.zeros(table.shape[1])]]
        stocks = np.append(stocks, np.sum(needs) - np.sum(stocks))
    
    print_matrix(table, needs, stocks)
    plan = reference_plan_calculation(table, needs, stocks)
    
    print_matrix(table, needs, stocks)
    
    while True:
        min_delta, min_delta_i, min_delta_j = get_delta(plan, table)
        if min_delta >= 0:
            break
        change_plan(plan, min_delta_i, min_delta_j)
        print_matrix(plan, needs, stocks)

    print('Final plan: ')
    for i in range(plan.shape[0]):
        for j in range(plan.shape[1]):
            if plan[i][j] == -1:
                plan[i][j] = 0
    
    print_matrix(plan, needs, stocks)
    print('Cost: ', np.sum(table * plan))

if __name__ == "__main__":    
    main()