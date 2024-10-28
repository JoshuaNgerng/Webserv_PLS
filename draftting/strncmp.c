#include <stdio.h>
#include <string.h>

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	while (*s1 == *s2 && *s1 != '\0' && *s2 != '\0' && n > 0)
	{
		s1++;
		s2++;
		n--;
	}
	if (n == 0)
		return (0);
	return (*(unsigned char *)s1 - *(unsigned char *)s2);
}

int	main(void)
{
	char	s1[] = "123456789";
	int		len1 = sizeof(s1);
	char	s2[] = "789";
	int		len2 = sizeof(s2);
	char	s3[] = "1234567890";
	int		len3 = sizeof(s3);

	printf("%s\n", &s1[len1 - len2]);
	printf("%s\n", &s3[len3 - len2]);
	printf("%d\n", ft_strncmp(&s1[len1 - len2], s2, len2));
	printf("%d\n", ft_strncmp(&s3[len3 - len2], s2, len2));
	return (0);
}