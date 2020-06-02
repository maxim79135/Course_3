import tkinter as tk
from tkinter.messagebox import showinfo
from hex_editor import HexEditor

class Page(tk.Frame):
	def __init__(self, *args, **kwargs):
		tk.Frame.__init__(self, *args, **kwargs)

	def show(self):
		self.lift()

class Page2(Page):
	def __init__(self, parent=None):
		self.parent = parent
		Page.__init__(self)

	def _build_ui(self):
		pass

class Page1(Page):
	def __init__(self, parent=None):
		self.parent = parent
		Page.__init__(self)
		#label = tk.Label(self, text="asd")
		self.params = [["Макс. требуемый объем памяти", "Dec"],
		["Мин. требуемый объем памяти", "Dec"],
		["Смещение Relocation Table", "Hex"],
		["Кол-во эл-тов в Relocation Table", "Dec"],
		["Кол-во параграфов в заголовке", "Dec"],
		["Смещение сегмента стека", "Hex"],
		["Смещение сегмента кода", "Hex"],
		["Значение указателя команд", "Hex"],
		["Длина загружаемой части", "Dec"]]
		self._completed_asks = 0
		self._build_ui()

	def _check_ans(self, text, row):
		text = text.upper()
		if row == 0 or row == 1 or row == 3 or row == 4 or row == 8:
			try:
				if int(text) != self.parent._ans_p1[row]:
					showinfo(title="Ошибка", message="Неправильно введенное число")
					self._canvases[row].itemconfig(1, fill="red")
				else:
					self._canvases[row].itemconfig(1, fill="green")
					self._completed_asks += 1
					self._widgets[row][2].config(state='disabled')
					self._widgets[row][2].config(font=('Helvetica', 10, 'bold'))
					if row < 8:
						self._widgets[row+1][2].focus_set()
			except ValueError:
				showinfo(title="Ошибка", message="Неправильно введенное число")
		else:
			if text not in self.parent._ans_p1[row]:
				showinfo(title="Ошибка", message="Неправильно введенное число")
				self._canvases[row].itemconfig(1, fill="red")
			else:
				self._canvases[row].itemconfig(1, fill="green")
				self._completed_asks += 1
				self._widgets[row][2].config(state='disabled')
				self._widgets[row][2].config(font=('Helvetica', 10, 'bold'))
				if row < 8:
					self._widgets[row+1][2].focus_set()
		
		if self._completed_asks == len(self.params):
			self.next_button.config(state="normal")
	
	def _build_ui(self):
		nameLabel = tk.Label(self, text="Заголовок EXE файла")
		nameLabel.pack(side="top")

		table = tk.Frame(self)
		self._widgets = []
		self._canvases = []
		for row in range(len(self.params)):
			current_row = []
			#labels
			for column in range(len(self.params[0])):
				label = tk.Label(table, text=self.params[row][column], borderwidth=0, anchor=tk.W, justify=tk.LEFT)
				label.grid(row=row, column=column, sticky="nsew", padx=10)
				current_row.append(label)

			#entry
			entry = tk.Entry(table, highlightcolor="blue")
			entry.insert(0, "Не задано")
			entry.grid(row=row, column=2, sticky="nsew")
			current_row.append(entry)

			#draw circle for check correct answers
			canvas = tk.Canvas(table, width="20", height="20")
			canvas.create_oval(4, 4, entry["width"] - 6, entry["width"] - 6, width=2)
			canvas.grid(row=row, column=3)
			current_row.append(canvas)
			self._widgets.append(current_row)
			self._canvases.append(canvas)

		for row in range(len(self.params)):
			self._widgets[row][2].bind("<Return>", lambda event, q=row: self._check_ans(self._widgets[q][2].get(), q))
			self._widgets[row][2].bind("<FocusIn>", lambda event, q=row: self._widgets[q][2].delete(0, tk.END))
			self._widgets[row][2].bind("<FocusOut>", lambda event, q=row: self._widgets[q][2].insert(0, "Не задано"))
		
		table.pack(side="top")

		#buttons next and exit
		exit_button = tk.Button(self, text="Выход", borderwidth=0, activeforeground="red", command=self.parent.parent.destroy)
		exit_button.pack(side="right", padx=10)

		self.next_button = tk.Button(self, text="Далее", state="disabled", borderwidth=0, activeforeground="red")
		self.next_button.pack(side="right")


class TestFrame(tk.Frame):
	def __init__(self, parent=None):
		super().__init__(master=parent)
		self.parent = parent
		self._build_ui()
		
		self._ans_p1 = self.parent._hexeditor.generate_ans_p1()
		
	def _build_ui(self):
		self.p2 = Page2(self)
		self.p1 = Page1(self)

		self.p1.pack(side="bottom",fill="x")


class MainWindow(tk.Tk):
	def __init__(self):
		super().__init__()

		screen_height = self.winfo_screenheight()
		self.geometry("430x280+{0}+{1}".format(150, screen_height // 2 - 100))
		self.resizable(False, False)
		self._build_ui()

	def _build_ui(self):
		self.title("Лабораторная работа по СисПО")
		
		self._hexeditor = HexEditor(self)

		self._testFrame = TestFrame(self)
		self._testFrame.pack(fill=tk.BOTH, expand=1)
		
	
