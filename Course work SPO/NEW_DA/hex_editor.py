import tkinter as tk
from random import randint

class HexEditor(tk.Toplevel):
	def __init__(self, parent=None):
		super().__init__(master=parent)
		self.screen_height = self.winfo_screenheight()
		self.resizable(False, False)
		self.prev_selected = (None, None)
		self.header = []
		self.dump = []
		self.create_header()
		self.add_zero_bytes(28*16+14)
		self._build_ui()
		

	def add_zero_bytes(self, count):
		self.dump += tuple('00' for _ in range(count))
	
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
		print(self.header[0x3] + self.header[0x2])
		ans.append(1024 - int(self.header[0x3] + self.header[0x2], 16))
		return ans

	def _build_ui(self):
		self.title("Просмотр кода")
		self.geometry("465x475+600+{0}".format(self.screen_height // 2 - 150))


		canvas = tk.Canvas(self, bg="blue")
		canvas.pack(side='left', expand=True)
		table = tk.Frame(canvas, bg="red")
		canvas.create_window((0, 0), window=table, anchor="s")
		canvas.bind('<Configure>', lambda event: canvas.configure(scrollregion=canvas.bbox('all')))

		scrollbar = tk.Scrollbar(self, orient='vertical', command=canvas.yview)
		scrollbar.pack(side='right', fill='y')
		canvas.configure(yscrollcommand = scrollbar.set)

		self._widgets = []
		for row in range(len(self.dump) // 16):
			current_row = []
			#labels
			for column in range(16):
				label = tk.Label(table, text=self.dump[row * 16 + column], borderwidth=0, anchor=tk.W, justify=tk.LEFT)
				label.grid(row=row, column=column, sticky="nsew", padx=3)
				current_row.append(label)

			self._widgets.append(current_row)

		for row in range(len(self.dump) // 16):
			for column in range(16):
				self._widgets[row][column].bind("<Button-1>", lambda event, q=row, p=column: self._select_byte(q, p))
				self._widgets[row][column].bind("<Left>", lambda event, q=row, p=column: self._move_left(q, p))

		toolbar = tk.Frame(self, bd=1, relief=tk.RAISED,bg="red")
		toolbar.pack(side=tk.LEFT, fill=tk.X, anchor="w")

		binary_text = tk.Label(self, text='Двоичное представление', padx=15)
		binary_text.pack(side=tk.BOTTOM, anchor="w")
		self.binary_byte = tk.Label(self)
		#self.binary_byte.pack(side=tk.LEFT)

	def _move_left(self, q, p):
		print(q, p)
		if p > 0:
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
		#self.binary_byte['text'] = format(self._widgets[q][p]['text'], '08b')
		if self.prev_selected != (None, None):
			self._widgets[self.prev_selected[0]][self.prev_selected[1]].config(fg='black')
		self.prev_selected = (q, p)

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
