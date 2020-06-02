import tkinter as tk

class HexEditor(tk.Toplevel):
	def __init__(self, parent=None):
		super().__init__(master=parent)
		self.screen_height = self.winfo_screenheight()
		self.resizable(False, False)
		self._build_ui()

	def _build_ui(self):
		self.title("Просмотр кода")
		self.geometry("465x475+600+{0}".format(self.screen_height // 2 - 150))

	def create_header(self):
		pass


