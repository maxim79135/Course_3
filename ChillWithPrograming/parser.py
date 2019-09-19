import urllib.request

fp = urllib.request.urlopen("https://vyatsu.antiplagiat.ru/report/summary/7971")
mybytes = fp.read()

mystr = mybytes.decode("utf8")
fp.close()

print(mystr)