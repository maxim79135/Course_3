import tkinter as tk
from tkinter.messagebox import showinfo
from hex_editor import HexEditor
import webbrowser
import os

class Page(tk.Frame):
	def __init__(self, *args, **kwargs):
		tk.Frame.__init__(self, *args, **kwargs)

	def show(self):
		self.lift()

class Page2(Page):
	def __init__(self, parent=None):
		self.parent = parent
		Page.__init__(self)
		self._completed_asks = 0
		self.changed = None
		self._build_ui()

	def add_command(self, event=None): 
		if self.ans.get() != '':
			if self.changed == None:
				self.lbox.insert(tk.END, self.ans.get())
			else:
				self.lbox.delete(self.changed)
				self.lbox.insert(self.changed, self.ans.get())
				self.changed = None
			self.ans.delete(0, tk.END)


	def delete_command(self):
		select = list(self.lbox.curselection())
		select.reverse()
		for i in select:
			self.lbox.delete(i)

	def change_command(self):
		select = list(self.lbox.curselection())
		text = self.lbox.get(select)
		self.ans.insert(0, text)
		self.changed = select

	def check_command(self):
		complete_commands = 0
		writing_commands = self.lbox.get(0, tk.END)
		consist_error = None
		for (i, com) in enumerate(writing_commands):
			if i < len(self.parent._ans_p2):
				print(i)
				if com not in self.parent._ans_p2[i]["asm"]:
					consist_error = True
				else:
					complete_commands += 1
		print(complete_commands)
		if consist_error:
			showinfo(title="Ошибка", message="Найдена ошибочная строка")
		if complete_commands == len(self.parent._ans_p2):
			showinfo(title="Успех", message="Все команды успешно дизассемблированы")
			self.parent.write_to_file()

	def left_command(self):
		select = list(self.lbox.curselection())
		if select and select[0] != 0:
			text = self.lbox.get(select)
			self.lbox.delete(select)
			self.lbox.insert(select[0] - 1, text)

	def right_command(self):
		select = list(self.lbox.curselection())
		if select and select[0] != len(self.lbox.get(0, tk.END)):
			text = self.lbox.get(select)
			self.lbox.delete(select)
			self.lbox.insert(select[0] + 1, text)

	def _build_ui(self):
		nameLabel = tk.Label(self, text="Декодирование команд", font=('Helvetica', 14, 'bold'), fg='blue')
		nameLabel.pack(side="top")

		self.lbox = tk.Listbox(self)
		self.lbox.pack(side=tk.LEFT, fill="both", expand=True, padx=10, pady=10)
		scroll = tk.Scrollbar(self, command=self.lbox.yview)
		scroll.pack(side=tk.LEFT, fill=tk.Y, pady=10)
		self.lbox.config(yscrollcommand=scroll.set)

		buttons_frame = tk.Frame(self)
		self.ans = tk.Entry(buttons_frame, highlightcolor="blue")
		self.ans.bind("<Return>", self.add_command)
		self.ans.pack(side="top")
		self.ans.focus_set()

		left_right = tk.Frame(buttons_frame)
		add_button = tk.Button(left_right, text="<", borderwidth=0, activeforeground="red", command=self.left_command)
		add_button.pack(side="left")

		add_button = tk.Button(left_right, text=">", borderwidth=0, activeforeground="red", command=self.right_command)
		add_button.pack(side="left")
		left_right.pack(side="top")

		add_button = tk.Button(buttons_frame, text="Добавить", borderwidth=0, activeforeground="red", command=self.add_command)
		add_button.pack(side="top")

		change_button = tk.Button(buttons_frame, text="Изменить", borderwidth=0, activeforeground="red", command=self.change_command)
		change_button.pack(side="top")

		delete_button = tk.Button(buttons_frame, text="Удалить", borderwidth=0, activeforeground="red", command=self.delete_command)
		delete_button.pack(side="top")

		delete_button = tk.Button(buttons_frame, text="Проверить", borderwidth=0, activeforeground="red", command=self.check_command)
		delete_button.pack(side="top")

		buttons_frame.pack(side="right", fill='x', expand=True)

class Page1(Page):
	def __init__(self, parent=None):
		self.parent = parent
		Page.__init__(self)
		self.params = [["Макс. требуемый объем памяти", "Dec"],
		["Мин. требуемый объем памяти", "Dec"],
		["Смещение Relocation Table", "Hex"],
		["Кол-во эл-тов в Relocation Table", "Dec"],
		["Кол-во параграфов в заголовке", "Dec"],
		["Смещение сегмента стека", "Hex"],
		["Смещение сегмента кода", "Hex"],
		["Значение указателя команд", "Hex"],
		["Длина загружаемой части", "Dec"]]
		self._completed_asks = 8
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
		nameLabel = tk.Label(self, text="Заголовок EXE файла", font=('Helvetica', 14, 'bold'), fg='blue')
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

		self.next_button = tk.Button(self, text="Далее", state="disabled", borderwidth=0, activeforeground="red", command=self.next_btn_click)
		self.next_button.pack(side="right")

	def next_btn_click(self):
		self.pack_forget()
		self.parent.p2.pack(side="bottom",fill="both", expand=True)
		self.parent.p2.show()

class TestFrame(tk.Frame):
	def __init__(self, parent=None):
		super().__init__(master=parent)
		self.parent = parent
		self._build_ui()
		self.write_to_file()
		
			
	def write_to_file(self):
		file = open('Ответы.txt', 'w')
		self._ans_p1 = self.parent._hexeditor.generate_ans_p1()
		self._ans_p2 = self.parent._hexeditor.generate_ans_p2()
		file.writelines('Задание 1:\n\n')
		for i, val in enumerate(self._ans_p1):
			file.writelines(str(i) + ': ' + str(val) + '\n')
		
		file.writelines('Задание 2:\n\n')
		for i, val in enumerate(self._ans_p2):
			file.writelines(str(i) + ': ' + str(val) + '\n')

		file.close()

	def _build_ui(self):
		self.p2 = Page2(self)
		self.p1 = Page1(self)

		#toolbar = tk.Frame(self, bd=1, relief=tk.RAISED)
		#exitButton = tk.Button(toolbar, text="1", relief=tk.FLAT, command= lambda: webbrowser.open_new('manuals.pdf'))
        #exitButton.image = eimg
		#exitButton.pack(side=tk.LEFT, padx=2, pady=2)
		#toolbar.pack(side=tk.TOP)

		self.p1.pack(side="bottom",fill="both")

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
		
	
