#include "test.h"

static void	compare(const char *s, int c)
{
	char	*got = ft_strchr(s, c);
	char	*exp = strchr(s, c);

	CHECK(got == exp, "s=%s c=%d: expected %s, got %s",
		show(s, strlen(s)), c,
		exp ? show(exp, strlen(exp)) : "NULL",
		got ? show(got, strlen(got)) : "NULL");
}

static void	found(void)
{
	compare("Hello, world!", 'o');
	compare("Hello, world!", 'H');
	compare("Hello, world!", '!');
}

static void	not_found(void)
{
	compare("Hello, world!", 'z');
}

static void	returns_first_occurrence(void)
{
	compare("abcabc", 'b');
	compare("aaaa", 'a');
}

static void	find_terminator(void)
{
	/* c == '\0' must return a pointer to the terminator, not NULL. */
	compare("Hello", '\0');
	compare("", '\0');
}

static void	empty_string(void)
{
	compare("", 'a');
}

static void	c_converted_to_char(void)
{
	compare("Hello", 'e' + 256);
	compare("Hello", 256);
	compare("ab\xff" "cd", -1);
	compare("ab\xff" "cd", 255);
	compare("ab\x80" "cd", 0x80);
}

static void	every_char_value(void)
{
	char	s[256];

	for (int i = 0; i < 255; i++)
		s[i] = (char)(255 - i);
	s[255] = '\0';
	for (int c = -1; c <= 256; c++)
		compare(s, c);
}

int	main(void)
{
	printf("ft_strchr\n");
	run_case("found at start, middle, end", found);
	run_case("not found -> NULL", not_found);
	run_case("returns FIRST occurrence", returns_first_occurrence);
	run_case("c = '\\0' returns pointer to terminator", find_terminator);
	run_case("empty string", empty_string);
	run_case("c converted to char ('e'+256, 256, -1, 255, 0x80)", c_converted_to_char);
	run_case("every c from -1 to 256 on a string of all bytes", every_char_value);
	return (finish("ft_strchr"));
}
