s = input()
stack = []
for v in s:
    if v == '(' or v == '[' or v == '{':
        stack.append(v)
    else:
        if v == ')' and len(stack) != 0 and stack[-1] == '(':
            stack.pop()
        elif v == ']' and len(stack) != 0 and stack[-1] == '[':
            stack.pop()
        elif v == '}' and len(stack) != 0 and stack[-1] == '{':
            stack.pop()

if len(stack) == 0:
    print('yes')
else:
    print('no')
