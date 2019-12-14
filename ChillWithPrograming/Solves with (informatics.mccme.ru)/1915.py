s = input().split(' ')
stack = []
for value in s:
	if value.isdigit():
		stack.append(value)
	else:
		tmp2 = int(stack.pop())
		tmp1 = int(stack.pop())
		if value == '-':
			stack.append(tmp1 - tmp2)
		elif value == '+':
			stack.append(tmp1 + tmp2)
		else:
			stack.append(tmp1 * tmp2)
print(stack[0])
