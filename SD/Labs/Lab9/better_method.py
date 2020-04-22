def _is_prime(value):
    import ctypes

    libcpp = ctypes.CDLL("./lib_external_method.so")

    return libcpp._is_prime(value)
