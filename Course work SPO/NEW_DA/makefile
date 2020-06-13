run:
	python3 main.py

clean:
	rm -rfv ./dist
	rm -rfv ./build

build_exe:
	pyinstaller -F -w --add-data 'manuals.pdf:.' main.py
