from xmlrpc.server import SimpleXMLRPCServer
from prime_numbers import is_prime, is_pairwise_primes, is_relative_primes, get_next, get_prev, get_random_prime_array, get_random_prime_number

def main():
    server = SimpleXMLRPCServer(("127.0.0.1", 8080))
    server.register_function(is_prime)
    server.register_function(is_pairwise_primes)
    server.register_function(is_relative_primes)
    server.register_function(get_next)
    server.register_function(get_prev)
    server.register_function(get_random_prime_array)
    server.register_function(get_random_prime_number)
    server.serve_forever()

if __name__ == "__main__":
    main()