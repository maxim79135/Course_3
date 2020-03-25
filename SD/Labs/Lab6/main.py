import prime_numbers as pn

def read_int(min_value, max_value):
    try:
        value = int(input())
        if min_value <= value <= max_value:
            return value
        return None
    except ValueError:
        return None

def menu():
    print("Possible options:")
    print("1. Get random prime number(from 2 to n)")
    print("2. Get random prime number(from m to n)")
    print("3. Get array of prime numbers")
    print("4. Is relative primes(numbers)")
    print("5. Is pairwise primes(numbers)")
    print("6. Get next prime following the number")
    print("7. Get previous prime following the number")
    print("8. Is prime(number)")
    print("9. Quit")
    print("Select item:", end=' ')

if __name__ == "__main__":
    while True:
        menu()
        op_code = read_int(1, 8)
        if op_code is None:
            print("Unknow command!\n\n")
            continue
        elif op_code == 8:
            value = read_int(1, 10**9)
            if value is None:
                print("Error")
            else:
                print(pn.sieveOfAtkin(value))
        elif op_code == 9:
            break