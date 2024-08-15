#include <iostream>

int	main(void)
{
	bool	t = true;
	if (t)
		std::cout << "check\n";
	else
		std::cout << "not check\n";
	int		i = static_cast<int>(t);
	if (t)
		std::cout << "check\n";
	else
		std::cout << "not check\n";
	std::cout << "int: " << i << '\n';
	std::cout << true << ", " << false << '\n';
	return (0);
}