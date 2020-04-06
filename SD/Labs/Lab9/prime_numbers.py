MAX_NUMBER = 999999937
MIN_NUMBER = 1

def _is_prime(number):
    from math import gcd, log, floor

    b = 13
    q = (2, 3, 5, 7, 11, 13)

    a = 5 % number
    for _ in range(10):
        while gcd(a, number) != 1:
            a = (a * a) % number
            a += 3
            a %= number

        for v in q:
            e = int(floor(log(b, v)))
            aa = int(pow(a, pow(v, e, number), number))

            if aa == 0:
                continue

            g = gcd(aa - 1, number)
            if 1 < g < number:
                return g == 1

    return True


def is_prime(number):
    return number == 2 or _is_prime(number)


def get_next(number, max_value):
    lrec = lambda n: -1 if n == 10 ** 9 or n > max_value else n if is_prime(n) else lrec(n + 1)
    return lrec(number)


def get_prev(number, min_value):
    lrec = lambda n: -1 if n == 1 or n < min_value else n if is_prime(n) else lrec(n - 1)
    return lrec(number)


def get_random_prime_number(m, n):
    from random import randint

    number = randint(m, n)
    if is_prime(number):
        return number
    else:
        u = get_next(number, n) 
        if u == -1:
            v = get_prev(number, m)
            if v == -1:
                return -1
            else:
                return v
        else:
            return u


def get_random_prime_array(length):
    return sorted(get_random_prime_number(2, MAX_NUMBER) for _ in range(length))


def is_relative_primes(numbers):
    from math import gcd
    result = numbers[0]
    for v in numbers[1:]:
        result = gcd(result, v)
    return True if result == 1 else False


def is_pairwise_primes(numbers):
    from math import gcd
    for i in range(len(numbers) - 1):
        for j in range(i + 1, len(numbers)):
            if gcd(numbers[i], numbers[j]) != 1:
                return False
    return True
