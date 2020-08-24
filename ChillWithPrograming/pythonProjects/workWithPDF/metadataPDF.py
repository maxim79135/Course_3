from PyPDF2 import PdfFileWriter, PdfFileReader

ipdf = PdfFileReader(open('old.pdf', 'rb'))
output = PdfFileWriter()

d = {"name": "Max"}

with open('new.pdf', 'wb') as f:
	output.addMetadata(d)
	output.write(f)

inp = PdfFileReader(open('new.pdf', 'rb'))
print(inp.getXmpMetadata())
#for i in range(ipdf.getNumPages()):
	#page = ipdf.getPage(i)
