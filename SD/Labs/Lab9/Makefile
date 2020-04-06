external_method.so: external_method.o
	g++ -shared -o lib_external_method.so external_method.o
external_method.o: external_method.cpp
	g++ -c -Wall -Werror -fpic external_method.cpp
clean: 
	rm external_method.o
