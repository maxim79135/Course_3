import ctypes

libcpp = ctypes.CDLL("./lib_external_method.so")

test = libcpp.test()
print(test)
