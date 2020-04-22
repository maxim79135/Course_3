#include <cmath>

int gcd(int a, int b) {
	int c;
	while (b) {
		a %= b;
		c = a;
		a = b;
		b = c;
	}
	return a;
}

extern "C" int _is_prime(int number) {
	int b = 13;
	int q[6] = {2, 3, 5, 7, 11, 13};

	int a = 5 % number;

	for (int i = 0; i < 10; i++) {
		while (gcd(a, number) != 1) {
			a = (a * a) % number;
			a += 3;
			a %= number;
		}	

		for (int i = 0; i < 6; i++) {
			int v = q[i];
			int e = int(floor(log(b) / log(v)));
			int aa = int((int)pow(a, int(pow(v, e)) % number) % number);

			if (aa == 0) continue;

			int g = gcd(aa - 1, number);
			if (g > 1 && g < number) return g == 1;
		}
	}
	return true;
}
