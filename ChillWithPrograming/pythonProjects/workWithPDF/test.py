from openpyxl import Workbook
from openpyxl.drawing.image import Image

wb = Workbook()
ws = wb.active
ws['A1'] = 'You should see three logos below'

img = Image('image.img')
ws.add_image(img, 'A2')
wb.save('logo.xlsx')
