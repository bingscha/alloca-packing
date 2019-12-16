int foo() {
	int a = 5;
	int sum = a + 10;
	sum <<= a;
	sum -= a;
	return sum;
}

int main() {foo(); return 0;}
