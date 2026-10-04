#include "test.h"

static void	compare(const char *s, int c)
{
	char	*got = ft_strrchr(s, c);
	char	*exp = strrchr(s, c);

	CHECK(got == exp, "s=%s c=%d: expected %s, got %s",
		show(s, strlen(s)), c,
		exp ? show(exp, strlen(exp)) : "NULL",
		got ? show(got, strlen(got)) : "NULL");
}

static void	found(void)
{
	compare("Hello, world!", 'w');
	compare("Hello, world!", '!');
}

static void	not_found(void)
{
	compare("Hello, world!", 'z');
}

static void	returns_last_occurrence(void)
{
	compare("abcabc", 'b');
	compare("aaaa", 'a');
	compare("Hello, world!", 'o');
}

static void	only_at_index_zero(void)
{
	/* Catches loops that stop before checking s[0]. */
	compare("Hello", 'H');
	compare("x", 'x');
}

static void	find_terminator(void)
{
	compare("Hello", '\0');
	compare("", '\0');
}

static void	empty_string(void)
{
	compare("", 'a');
}

static void	c_converted_to_char(void)
{
	compare("Hello", 'l' + 256);
	compare("Hello", 256);
	compare("ab\xff" "cd\xff", -1);
	compare("ab\xff" "cd", 255);
}

static void	every_char_value(void)
{
	char	s[511];

	for (int i = 0; i < 255; i++)
	{
		s[i] = (char)(255 - i);
		s[i + 255] = (char)(255 - i);
	}
	s[510] = '\0';
	for (int c = -1; c <= 256; c++)
		compare(s, c);
}

int	main(void)
{
	printf("ft_strrchr\n");
	run_case("found", found);
	run_case("not found -> NULL", not_found);
	run_case("returns LAST occurrence", returns_last_occurrence);
	run_case("match only at s[0]", only_at_index_zero);
	run_case("c = '\\0' returns pointer to terminator", find_terminator);
	run_case("empty string", empty_string);
	run_case("c converted to char ('l'+256, 256, -1, 255)", c_converted_to_char);
	run_case("every c from -1 to 256 on a string of all bytes", every_char_value);
	return (finish("ft_strrchr"));
}
