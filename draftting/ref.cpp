#include <iostream>

int	main(void) {
	int	s[100];
	for (int i = 0; i < 100; i ++) {
		s[i] = i * 2;
	}
	for (int i = 0; i < 10; i ++) {
		int	&ref = s[i];
		ref = i;
		std::cout << s[i] << ", " << &ref << '\n';
	}
	std::cout << s[10] << '\n';
	std::cout << s[5] << '\n';
	return (0);
}
