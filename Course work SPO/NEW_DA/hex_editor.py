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
		self.add_zero_bytes(50*16+14)
		self._build_ui()
		

	def add_zero_bytes(self, count):
		self.dump += tuple('00' for _ in range(count))

	def _build_ui(self):
		self.title("Просмотр кода")
		self.geometry("465x475+600+{0}".format(self.screen_height // 2 - 150))


		canvas = tk.Canvas(self)
		canvas.pack(side='top', expand="yes")
		table = tk.Frame(canvas)
		table.pack(side="left")


		scrollbar = tk.Scrollbar(self, orient='vertical')
		scrollbar.pack(side='right', fill='y')
		scrollbar['command'] = canvas.yview
		canvas['yscrollcommand'] = scrollbar.set


		self._widgets = []
		for row in range(len(self.dump) // 16):
			current_row = []
			#labels
			for column in range(16):
				label = tk.Label(table, text=self.dump[row * 16 + column], borderwidth=0, anchor=tk.W, justify=tk.LEFT)
				label.grid(row=row, column=column, sticky="nsew", padx=5)
				current_row.append(label)

			self._widgets.append(current_row)

		for row in range(len(self.dump) // 16):
			for column in range(16):
				self._widgets[row][column].bind("<Button-1>", lambda event, q=row, p=column: self._select_byte(q, p))

		toolbar = tk.Frame(self, bd=1, relief=tk.RAISED)
		toolbar.pack(side=tk.BOTTOM, fill=tk.X)

		binary_text = tk.Label(toolbar, text='Двоичное представление', padx=15)
		binary_text.pack(side=tk.LEFT)
		self.binary_byte = tk.Label(toolbar)
		self.binary_byte.pack(side=tk.LEFT)


	def _select_byte(self, q, p):
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
