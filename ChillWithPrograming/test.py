a = 1
b = 1000

stack = []
for value in range(a, b + 1):
    stack.append(value)
    if value % 10 == 0:
        if len(stack) < 2 ** (value / 10 - 1):
            break
        for i in range(2 ** (value // 10 - 1)):
            stack.pop()

print(stack)
