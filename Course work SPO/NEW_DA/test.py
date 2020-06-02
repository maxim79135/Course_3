from tkinter import *
  
  
class Table: 
      
    def __init__(self,root): 
          
        # code for creating table 
        for i in range(total_rows): 
            for j in range(total_columns): 
                  
                self.e = Label(root, text=lst[i][j], width=20, fg='blue', 
                               font=('Arial',16,'bold')) 
                  
                self.e.grid(row=i, column=j)
                self.e.bind("<Button-1>", self.click_label)
                #self.e.bind("<ButtonRelease-1>", self.release_label)
    
    def release_label(self, event):
        print(event.widget.config(bg="blue"))

    def click_label(self, event):
        print(event.widget.config(fg="red"))


# take the data 
lst = [(1,'Raj','Mumbai',19), 
       (2,'Aaryan','Pune',18), 
       (3,'Vaishnavi','Mumbai',20), 
       (4,'Rachna','Mumbai',21), 
       (5,'Shubham','Delhi',21)] 
   
# find total number of rows and 
# columns in list 
total_rows = len(lst) 
total_columns = len(lst[0]) 
   
# create root window 
root = Tk() 
t = Table(root) 
root.mainloop() 