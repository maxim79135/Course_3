s = input()
numbers = ['0', '1','2', '3', '4','5', '6', '7', '8', '9']
operations = ['+', '-', '(', ')', '*', '/']
buf = ''
for i in range(len(s)):
	if s[i] in numbers:
		buf += s[i]
		#print(s[i], end='')
	elif i > 0 and s[i - 1] in numbers and s[i] not in numbers:
		print(buf)
		buf = ''
	if s[i] not in numbers and s[i] != ' ':
		print(s[i])
print(buf)
