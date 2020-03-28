import tkinter as tk
import prime_numbers as pn
from tkinter import messagebox


MIN_VALUE = 1
MAX_VALUE = 10**9

def read_int(string, min_value, max_value):
    try:
        value = int(string)
        if min_value <= value <= max_value:
            return value
        return None
    except ValueError:
        return None


def read_int_array(s, min_value, max_value):
    values = s.split(' ')
    numbers = []
    for i in values:
        v = read_int(i, min_value, max_value)
        if v is None:
            return None
        else:
            numbers.append(v)
    return numbers


class ReadOnlyText(tk.Text):
    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs)

        self['state'] = tk.DISABLED

    def set_text(self, text: str):
        self['state'] = tk.NORMAL
        self.delete('0.0', tk.END)
        self.insert('0.0', text)
        self['state'] = tk.DISABLED


class MainFrame(tk.Frame):
    def __init__(self, parent=None):
        super().__init__(master=parent)

        self._build_ui()
    
    def _build_ui(self):
        rbuttons_frame = tk.Frame(self)
        type_label = tk.Label(rbuttons_frame, text='Выбор команды:')
        type_label.pack(side=tk.TOP)

        self._type_var = tk.StringVar()
        self._op_code = tk.StringVar()
        self._op_code = "3"
        self.random_array = tk.Radiobutton(rbuttons_frame, text='Случайный массив', variable=self._type_var, value='random_array', command=self.change_name_method)
        self.random_array.select()
        self.random_array.pack(side=tk.TOP)

        self.random_prime_mn = tk.Radiobutton(rbuttons_frame, text='Случайное число (m, n)', variable=self._type_var, value='random_prime_mn', command=self.change_name_method)
        self.random_prime_mn.pack(side=tk.TOP)

        self.is_relative = tk.Radiobutton(rbuttons_frame, text='Взаимно-простые числа', variable=self._type_var, value='is_relative', command=self.change_name_method)
        self.is_relative.pack(side=tk.TOP)

        self.is_pairwise = tk.Radiobutton(rbuttons_frame, text='Попарно-простые числа', variable=self._type_var, value='is_pairwise', command=self.change_name_method)
        self.is_pairwise.pack(side=tk.TOP)

        self.get_next = tk.Radiobutton(rbuttons_frame, text='Следующее простое число', variable=self._type_var, value='get_next', command=self.change_name_method)
        self.get_next.pack(side=tk.TOP)

        self.get_prev = tk.Radiobutton(rbuttons_frame, text='Предыдущее простое число', variable=self._type_var, value='get_prev', command=self.change_name_method)
        self.get_prev.pack(side=tk.TOP)    

        self.is_prime = tk.Radiobutton(rbuttons_frame, text='Проверка на простоту', variable=self._type_var, value='is_prime', command=self.change_name_method)
        self.is_prime.pack(side=tk.TOP)

        rbuttons_frame.pack(side=tk.LEFT, fill=tk.X)        


        #params frame
        params_frame = tk.Frame(self)
        self.name_method_label = tk.Label(params_frame, text='Случайный массив', width=25)
        self.name_method_label.pack(side=tk.TOP)

        self._command_var = tk.StringVar()
        self.command_entry = tk.Entry(params_frame, textvariable=self._command_var, width=20)
        self.command_entry.pack(side=tk.TOP)

        execute_command_button = tk.Button(params_frame, text='Выполнить команду', command=self._execute_command)
        execute_command_button.pack(side=tk.TOP)
        params_frame.pack(side=tk.LEFT)
        

        #result frame by command
        result_frame = tk.Frame(self)

        scrollbar = tk.Scrollbar(result_frame)
        scrollbar.pack(side=tk.RIGHT, fill=tk.Y)

        self._result_text = ReadOnlyText(result_frame, yscrollcommand=scrollbar.set)
        scrollbar['command'] = self._result_text.yview
        self._result_text.pack(side=tk.LEFT)

        result_frame.pack(side=tk.TOP)

    def _execute_command(self):
        s = self._command_var.get()
        op_code = int(self._op_code)
        if op_code == 2:
            l = read_int_array(s, MIN_VALUE, MAX_VALUE)
            if l is None or len(l) != 2 or l[0] > l[1]:
                messagebox.showerror('Error', 'Wrong numbers')
            else:
                m, n = l[0], l[1]
                self._result_text.set_text(pn.get_random_prime_number(m, n))

        elif op_code == 3:
            value = read_int(s, MIN_VALUE, MAX_VALUE)
            if value is None:
                messagebox.showerror('Error', 'Wrong length of array')
            else:
                array = pn.get_random_prime_array(value)
                self._result_text.set_text(self._dump_json(array))

        elif op_code == 4:
            l = read_int_array(s, MIN_VALUE, MAX_VALUE)
            if l is None:
                messagebox.showerror('Error', 'Wrong array')
            else:
                self._result_text.set_text(pn.is_relative_primes(l))

        elif op_code == 5:
            l = read_int_array(s, MIN_VALUE, MAX_VALUE)
            if l is None:
                messagebox.showerror('Error', 'Wrong array')
            else:
                self._result_text.set_text(pn.is_pairwise_primes(l))

        elif op_code == 6:
            value = read_int(s, MIN_VALUE, MAX_VALUE)
            if value is None:
                messagebox.showerror('Error', 'Wrong number')
            else:
                self._result_text.set_text(pn.get_next(value, MAX_VALUE))

        elif op_code == 7:
            value = read_int(s, MIN_VALUE, MAX_VALUE)
            if value is None:
                messagebox.showerror('Error', 'Wrong number')
            else:
                self._result_text.set_text(pn.get_prev(value, MIN_VALUE))

        elif op_code == 8:
            value = read_int(s, MIN_VALUE, MAX_VALUE)
            if value is None:
                messagebox.showerror('Error', 'Value must be in range(1, 10^9)')
            else:
                self._result_text.set_text(pn.is_prime(value))

    @staticmethod
    def _dump_json(array):
        from json import dumps

        return dumps(array, indent=2)

    def change_name_method(self):
        self.command_entry.delete(0, 'end')
        if self._type_var.get() == "random_prime_mn":
            self.name_method_label['text'] = self.random_prime_mn['text']
            self._op_code = "2"
        elif self._type_var.get() == "random_array":
            self.name_method_label['text'] = self.random_array['text']
            self._op_code = "3"
        elif self._type_var.get() == "is_relative":
            self.name_method_label['text'] = self.is_relative['text']
            self._op_code = "4"
        elif self._type_var.get() == "is_pairwise":
            self.name_method_label['text'] = self.is_pairwise['text']
            self._op_code = "5"
        elif self._type_var.get() == "get_next":
            self.name_method_label['text'] = self.get_next['text']
            self._op_code = 6
        elif self._type_var.get() == "get_prev":
            self.name_method_label['text'] = self.get_prev['text']
            self._op_code = 7
        elif self._type_var.get() == "is_prime":
            self.name_method_label['text'] = self.is_prime['text']
            self._op_code = 8

class NumberFrame(tk.Frame):
    def __init__(self, parent=None):
        super().__init__(master=parent)

        self._build_ui()
    
    def _build_ui(self):
        gen_number_bt = tk.Button(self, text='Случайное простое число', command=self._gen_number)
        gen_number_bt.pack(side=tk.LEFT)

        self._number_var = tk.StringVar()
        number_entry = tk.Entry(self, textvariable=self._number_var, state='readonly')
        number_entry.pack(side=tk.LEFT)
    
    def _gen_number(self):
        self._number_var.set(pn.get_random_prime_number(2, pn.MAX_NUMBER))

class Application(tk.Tk):
    def __init__(self):
        super().__init__()

        self._build_ui()
    
    def _build_ui(self):
        self.title('Prime numbers')

        self._number_frame = NumberFrame(self)
        self._number_frame.pack(side=tk.TOP)

        self._main_frame = MainFrame(self)
        self._main_frame.pack(side=tk.TOP)

if __name__ == "__main__":
    app = Application()
    app.mainloop()