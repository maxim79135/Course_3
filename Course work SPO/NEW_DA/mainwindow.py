import tkinter as tk
from tkinter.messagebox import showinfo
from hex_editor import HexEditor

class Page(tk.Frame):
	def __init__(self, *args, **kwargs):
		tk.Frame.__init__(self, *args, **kwargs)

	def show(self):
		self.lift()

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
		self._build_ui()

	def _check_ans(self, text, row):
		if text != "123":
			showinfo(title="Ошибка", message="Неправильно введенное число")

	def _build_ui(self):
		nameLabel = tk.Label(self, text="Заголовок EXE файла")
		nameLabel.pack(side="top")

		table = tk.Frame(self)
		self._widgets = []
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
			canvas.create_oval(0, 0, entry["width"] - 10, entry["width"] - 10, width=2)
			canvas.grid(row=row, column=3)
			current_row.append(canvas)
			self._widgets.append(current_row)

		for row in range(len(self.params)):
			self._widgets[row][2].bind("<Return>", lambda event, q=row: self._check_ans(self._widgets[q][2].get(), q))
		
		table.pack(side="top")

		#buttons next and exit
		exitButton = tk.Button(self, text="Выход", borderwidth=0, activeforeground="red", command=self.parent.parent.destroy)
		exitButton.pack(side="right", padx=10)

		nextButton = tk.Button(self, text="Далее", state="disabled")
		nextButton.pack(side="right")


class TestFrame(tk.Frame):
	def __init__(self, parent=None):
		super().__init__(master=parent)
		self.parent = parent
		self._build_ui()

	def _build_ui(self):
		p1 = Page1(self)

		p1.pack(side="bottom",fill="x")



class MainWindow(tk.Tk):
	def __init__(self):
		super().__init__()

		screen_height = self.winfo_screenheight()
		self.geometry("430x280+{0}+{1}".format(150, screen_height // 2 - 100))
		self.resizable(False, False)
		self._build_ui()

	def _build_ui(self):
		self.title("Лабораторная работа по СисПО")

		self._testFrame = TestFrame(self)
		self._testFrame.pack(fill=tk.BOTH, expand=1)
		
		self._hexeditor = HexEditor(self)
