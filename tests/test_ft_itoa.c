#include "test.h"

/* The expected string comes from snprintf("%d"). */
static void	expect(int n)
{
	char	exp[16];
	char	*got = ft_itoa(n);

	snprintf(exp, sizeof(exp), "%d", n);
	CHECK(got != NULL, "n=%d: returned NULL", n);
	if (!got)
		return ;
	CHECK(strcmp(got, exp) == 0, "n=%d: expected \"%s\", got %s",
		n, exp, show(got, strlen(got)));
	free(got);
}

static void	zero(void)
{
	expect(0);
}

static void	single_digits(void)
{
	for (int n = -9; n <= 9; n++)
		expect(n);
}

static void	positive(void)
{
	expect(42);
	expect(12345);
	expect(1000000);
}

static void	negative(void)
{
	expect(-42);
	expect(-12345);
	expect(-1000000);
}

static void	powers_of_ten(void)
{
	/* Off-by-one digit counting shows up at 10, 100, ... */
	for (int p = 1; p <= 1000000000; p *= 10)
	{
		expect(p);
		expect(p - 1);
		expect(-p);
		expect(-p + 1);
		if (p == 1000000000)
			break ;
	}
}

static void	int_max(void)
{
	expect(INT_MAX);
	expect(INT_MAX - 1);
}

static void	int_min(void)
{
	/* -INT_MIN overflows an int: must be handled specially. */
	expect(INT_MIN);
	expect(INT_MIN + 1);
}

static void	range_sweep(void)
{
	for (int n = -10000; n <= 10000; n++)
		expect(n);
	for (int n = INT_MAX - 1000; n < INT_MAX; n++)
		expect(n);
	for (int n = INT_MIN; n < INT_MIN + 1000; n++)
		expect(n);
}

static void	new_string_each_call(void)
{
	char	*a = ft_itoa(1);
	char	*b = ft_itoa(1);

	CHECK(a && b && a != b, "two calls returned the same pointer");
	free(a);
	free(b);
}

int	main(void)
{
	printf("ft_itoa\n");
	run_case("0 -> \"0\"", zero);
	run_case("single digits -9..9", single_digits);
	run_case("positive numbers", positive);
	run_case("negative numbers", negative);
	run_case("powers of ten and their neighbours", powers_of_ten);
	run_case("INT_MAX", int_max);
	run_case("INT_MIN (-2147483648)", int_min);
	run_case("sweep -10000..10000 and near INT_MAX / INT_MIN", range_sweep);
	run_case("each call returns a new string", new_string_each_call);
	return (finish("ft_itoa"));
}
