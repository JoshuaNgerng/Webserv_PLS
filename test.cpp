#include <iostream>
class Obj {
	;
};
int main(void) {
	Obj	A;
	Obj	B;
	Obj	&ref = A;
	std::cout << &ref << '\n';
	ref = B;
	std::cout << &ref << '\n';
	std::cout << &A << '\n';
	std::cout << &B << '\n';
}