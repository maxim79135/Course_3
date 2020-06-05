run:
	python3 main.py

run_exe:
	wine ~/Загрузки/python_win/python.exe main.py
clean:
	rm -rfv ./dist

build_exe:
	pyinstaller -F -w main.py
	cp /dist/main /dist/main.pp
