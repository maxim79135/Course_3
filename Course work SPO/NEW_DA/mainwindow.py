import tkinter as tk

class MainWindow(tk.Tk):
	def __init__(self):
		super().__init__()

		self._build_ui()

	def _build_ui(self):
		self.title("Лабораторная работа по СисПО")
