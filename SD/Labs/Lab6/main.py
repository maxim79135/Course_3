import prime_numbers as pn

MIN_VALUE = 1
MAX_VALUE = 10**9

def read_int(string, min_value, max_value):
    try:
        value = int(string)
        if min_value <= value <= max_value:
            return value
        return None
    except ValueError:
        return None


def read_int_array(s, min_value, max_value):
    values = s.split(' ')
    numbers = []
    for i in values:
        v = read_int(i, min_value, max_value)
        if v is None:
            return None
        else:
            numbers.append(v)
    return numbers


def menu():
    print("Possible options:")
    print("1. Get random prime number(from 2 to 10^9)")
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
        s = input()
        op_code = read_int(s, MIN_VALUE, MAX_VALUE)
        if op_code is None:
            print("Unknow command!\n\n")
            continue
        
        elif op_code == 1:
            print(pn.get_random_prime_number(2, MAX_VALUE))
            input("Enter for continue...")
        
        elif op_code == 2:
            s = input("Enter min, max values: ")
            l = read_int_array(s, MIN_VALUE, MAX_VALUE)
            if l is None or len(l) != 2 or l[0] > l[1]:
                print("Wrong numbers")
            else:
                m, n = l[0], l[1]
                print(pn.get_random_prime_number(m, n))
            input("Enter for continue...")
        
        elif op_code == 3:
            s = input("Enter length of array: ")
            value = read_int(s, MIN_VALUE, MAX_VALUE)
            if value is None:
                print("Wrong length of array")
            else:
                array = pn.get_random_prime_array(value)
                print(*array, sep=' ')
            input("Enter for continue...")

        elif op_code == 4:
            s = input("Enter array values: ")
            l = read_int_array(s, MIN_VALUE, MAX_VALUE)
            if l is None:
                print("Wrong array")
            else:
                print(pn.is_relative_primes(l))
            input("Enter for continue...")

        elif op_code == 5:
            s = input("Enter array values: ")
            l = read_int_array(s, MIN_VALUE, MAX_VALUE)
            if l is None:
                print("Wrong array")
            else:
                print(pn.is_pairwise_primes(l))
            input("Enter for continue...")

        elif op_code == 6:
            s = input("Enter value: ")
            value = read_int(s, MIN_VALUE, MAX_VALUE)
            if value is None:
                print("Wrong number")
            else:
                print(pn.get_next(value, MAX_VALUE))
            input("Enter for continue...")

        elif op_code == 7:
            s = input("Enter value: ")
            value = read_int(s, MIN_VALUE, MAX_VALUE)
            if value is None:
                print("Wrong number")
            else:
                print(pn.get_prev(value, MIN_VALUE))
            input("Enter for continue...")

        elif op_code == 8:
            s = input()
            value = read_int(s, MIN_VALUE, MAX_VALUE)
            if value is None:
                print("Value must be in range(1, 10^9)")
            else:
                print(pn.is_prime(value))
            input("Enter for continue...")
        
        elif op_code == 9:
            break