import tkinter as tk
from random import randint

class HexEditor(tk.Toplevel):
	def __init__(self, parent=None):
		super().__init__(master=parent)
		
		self.tests = [
			{"asm": ["mov bx, 0417h", "mov bx,0417h"], "dasm": ("BB", "17", "04")},
			{"asm": ["xor ax, ax", "xor ax,ax"], "dasm": ("33", "C0")},	
			{"asm": "pop ss", "dasm": ["17"]},
			{"asm": "int 21h", "dasm": ("CD", "21")},
			{"asm": ["mov ax, 4c00h", "mov ax,4c00h"], "dasm": ("B8", "00", "4C")},
		]

		self.moves = [{"asm": "mov ax, bx", "dasm": ("89", "D8")},
		{"asm": "xor ax, ax", "dasm": ("33", "C0")},
		{"asm": "push ax", "dasm": ["50"]},
		{"asm": "push cx", "dasm": ["51"]},
		{"asm": "push dx", "dasm": ["52"]},
		{"asm": "push bx", "dasm": ["53"]},
		{"asm": "push ds", "dasm": ["1E"]},
		{"asm": "push es", "dasm": ["06"]},
		{"asm": "push ss", "dasm": ["16"]},
		{"asm": "push cs", "dasm": ["0E"]},
		{"asm": "mov ax, 0h", "dasm": ("B8", "00", "00")},
		{"asm": "mov cx, 0h", "dasm": ("B9", "00", "00")},
		{"asm": "mov dx, 0h", "dasm": ("BA", "00", "00")},
		{"asm": "mov bx, 0h", "dasm": ("BB", "00", "00")},
		{"asm": "mov ds, ax", "dasm": ("8E", "D8")},
		{"asm": "mov ah, 9h", "dasm": ("B4", "09")},
		{"asm": "mov ch, 5h", "dasm": ("B5", "05")},
		{"asm": "mov dh, 4h", "dasm": ("B6", "04")},
		{"asm": "mov bh, 6h", "dasm": ("B7", "06")},
		{"asm": "pop ax", "dasm": ["58"]},
		{"asm": "pop cx", "dasm": ["59"]},
		{"asm": "pop dx", "dasm": ["5A"]},
		{"asm": "pop bx", "dasm": ["5B"]},
		{"asm": "pop ds", "dasm": ["1F"]},
		{"asm": "pop es", "dasm": ["07"]},
		{"asm": "pop ss", "dasm": ["17"]},
		{"asm": "int 21h", "dasm": ("CD", "21")},
		{"asm": "int 29h", "dasm": ("CD", "29")},
		{"asm": "mov bx, 0417h", "dasm": ("BB", "17", "04")},
		{"asm": "mov ax, 4c00h", "dasm": ("B8", "00", "4C")},
		{"asm": "mov al, [bx]", "dasm": ("8A", "07")},
		{"asm": "add al, 30h", "dasm": ("04", "30")},
		{"asm": "add al, 40h", "dasm": ("04", "40")},
		{"asm": "add al, 5h", "dasm": ("04", "05")},
		
		{"asm": "retf", "dasm": ["CB"]}
		]
		
		self.screen_height = self.winfo_screenheight()
		self.resizable(False, False)
		self.prev_selected = (None, None)
		self.commands = []
		#self.create_dump()
		self.generate_test()
		self._build_ui()
		
	def generate_ans_p1(self):
		ans = []
		ans += (65535, 0) 
		ans.append([self.header[0x19] + self.header[0x18] + 'H', \
					'0X' + self.header[0x19] + self.header[0x18]])
		ans += (1, 0x20)
		ans.append([self.header[0xF] + self.header[0xE] + 'H', \
					'0X' + self.header[0xF] + self.header[0xE]])
		ans.append([self.header[0x17] + self.header[0x16] + 'H', \
					'0X' + self.header[0x17] + self.header[0x16]])
		ans.append([self.header[0x15] + self.header[0x14] + 'H', \
					'0X' + self.header[0x15] + self.header[0x14]])
		ans.append(1024 - int(self.header[0x3] + self.header[0x2], 16))
		return ans

	def generate_ans_p2(self):
		return self.commands

	def _build_ui(self):
		self.title("Просмотр кода")
		self.geometry("500x475+600+{0}".format(self.screen_height // 2 - 150))

		toolbar = tk.Frame(self, bd=1, relief=tk.RAISED)
		toolbar.pack(side=tk.BOTTOM, fill=tk.X)

		binary_text = tk.Label(toolbar, text='Двоичное представление')
		binary_text.pack(side=tk.LEFT, anchor="w")
		self.binary_byte = tk.Label(toolbar)
		self.binary_byte.pack(side=tk.LEFT, padx=7)
		self.next_binary_byte = tk.Label(toolbar)
		self.next_binary_byte.pack(side=tk.LEFT, padx=7)

		canvas = tk.Canvas(self)
		scrollbar = tk.Scrollbar(self, orient='vertical', command=canvas.yview)
		
		scrollbar.pack(side='right', fill='y')
		canvas.pack(side='right', expand=True, fill='both')		

		table = tk.Frame(canvas)
		canvas.create_window((0, 0), window=table)
		canvas.bind('<Configure>', lambda event: canvas.configure(scrollregion=canvas.bbox('all')))
		canvas.configure(yscrollcommand = scrollbar.set)
	
		self._widgets = []
		
		for column in range(16):
			label = tk.Label(table, text='{:02X}'.format(column), borderwidth=0, anchor=tk.W, justify=tk.LEFT, fg='blue')
			label.grid(row=0, column=column+1, sticky="nsew", padx=3)

		for row in range(len(self.dump) // 16):
			current_row = []
			#labels
			label = tk.Label(table, text='{:04X}'.format(16 * row) + ':', borderwidth=0, anchor=tk.W, justify=tk.LEFT, fg='green')
			label.grid(row=row+1, column=0, sticky="nsew", padx=7)
			current_row.append(label)
			for column in range(16):
				label = tk.Label(table, text=self.dump[row * 16 + column], borderwidth=0, anchor=tk.W, justify=tk.LEFT)
				label.grid(row=row+1, column=column+1, sticky="nsew", padx=3)
				current_row.append(label)

			self._widgets.append(current_row)

		for row in range(len(self.dump) // 16):
			for column in range(16):
				self._widgets[row][column+1].bind("<Button-1>", lambda event, q=row, p=column+1: self._select_byte(q, p))
				#self._widgets[row][column].bind("<Left>", lambda event, q=row, p=column: self._move_left(q, p))

	def _move_left(self, q, p):
		print(q, p)
		if p > 1:
			self._widgets[q][p - 1].config(fg='red')
			_q = q
			_p = p - 1
		else:
			self._widgets[q - 1][15].config(fg='red')
			_q = q - 1
			_p = 15
		self._widgets[_q][_p].focus_set()
		if self.prev_selected != (None, None):
			self._widgets[self.prev_selected[0]][self.prev_selected[1]].config(fg='black')
		self.prev_selected = (q, p)
		print(self.prev_selected)

	def _select_byte(self, q, p):
		self._widgets[q][p].focus_set()
		self._widgets[q][p].config(fg='red')
		self.binary_byte['text'] = format(int('0x' + self._widgets[q][p]['text'], 16), '08b')
		if p + 1 == 17:
			self.next_binary_byte['text'] = format(int('0x' + self._widgets[q+1][1]['text'], 16), '08b')
		else:
			self.next_binary_byte['text'] = format(int('0x' + self._widgets[q][p+1]['text'], 16), '08b')
		if self.prev_selected != (None, None):
			self._widgets[self.prev_selected[0]][self.prev_selected[1]].config(fg='black')
		self.prev_selected = (q, p)

	def str_to_int(self, s):
		return int('0x' + s, 16)

	def add_zero_bytes(self, count):
		self.dump += tuple('00' for _ in range(count))
	
	def add_rand_bytes(self, count):
		self.dump += tuple(format(randint(100, 200), 'X') for _ in range(count))

	def create_dump(self):
		self.header = []
		self.dump = []
		self.create_header()
		self.add_zero_bytes(28*16+14)
		self.create_main()

	def create_header(self):
		self.header += ('4D', '5A')
		self.header += (format(randint(50, 200), 'X'), '00')
		self.header += ('03', '00')
		self.header += ('01', '00')
		self.header += ('20', '00')
		self.header += ('00', '00')
		self.header += ('FF', 'FF')
		self.header += (format(randint(100, 200), 'X'), '00')
		self.header += (format(randint(16, 255), 'X'), '01')
		self.header += ('00', '00')
		self.header += (format(randint(16, 255), 'X'), '01')
		self.header += ('01', '00')
		self.header += ('3E', '00')
		self.header += ('00', '00')
		self.header += ('01', '00')
		self.header += ('FB', '50')
		self.header += ('6A', '72')
		self.header += tuple('00' for _ in range(28))
		self.header += ('35', '01')
		self.header += ('01', '00')

		self.dump += self.header

	def rand_command(self):
		com = self.moves[randint(0, len(self.moves) - 2)]
		self.commands.append(com)
		return com["dasm"]

	def create_main(self):
		start = (self.str_to_int(self.header[0x9] + self.header[0x8]) + \
			self.str_to_int(self.header[0x17] + self.header[0x16])) * 16 + \
				self.str_to_int(self.header[0x15] + self.header[0x14])
		print(hex(start))
		self.add_rand_bytes(start - 0x210)

		for _ in range(15):
			self.dump += self.rand_command()
		self.commands += self.moves[-1]
		self.dump += self.moves[-1]["dasm"]
		self.add_rand_bytes(60)
		
	def generate_test(self):
		self.header = []
		self.dump = []
		self.create_header()
		self.add_zero_bytes(28*16+14)
		start = (self.str_to_int(self.header[0x9] + self.header[0x8]) + \
			self.str_to_int(self.header[0x17] + self.header[0x16])) * 16 + \
				self.str_to_int(self.header[0x15] + self.header[0x14])
		print(hex(start))
		self.add_rand_bytes(start - 0x210)

		for i in range(len(self.tests)):
			self.dump += self.tests[i]["dasm"]
			self.commands.append(self.tests[i])
		self.commands.append(self.moves[-1])
		self.dump += self.moves[-1]["dasm"]
		self.add_rand_bytes(60)

