#include <assert.h>
#include <float.h>
#include <limits.h>
#include <locale.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char
  *reset = "\033[0m",
   *bold = "\033[1m",
    *red = "\033[31m",
  *green = "\033[32m",
   *blue = "\033[34m",
*magenta = "\033[35m";

static int success = 0, failure = 0;
static char *str;

void printc(const char *color, const char *stat, const char *msg) {
	const char *hl = reset;

	if (strcmp(color, magenta) == 0)
		hl = magenta;

	printf("%*s%s%-*s%s%s%s%s\n", 10 - (int)strlen(stat), bold, color,
		(int)(strlen(stat) + 1), stat, reset, hl, msg, reset);
}

void prints(const char *color, const char *msg, int value) {
	char style[strlen(bold) + strlen(color) + 1];
	sprintf(style, "%s%s", bold, color);

	printf("%s%11s%s: %d%s\n", style, msg, reset, value, reset);
}

#define count(e_size, fmt, ...) ({ \
	char *str2 = NULL; \
	int size = asprintf(&str2, fmt, __VA_ARGS__); \
	assert(str2);	\
	assert(e_size == size); \
	free(str2); \
})

int test(int cond, const char *msg) {
	str = NULL;

	if (!cond) {
		failure++;
		printc(magenta, "Err", msg);
		return EXIT_FAILURE;
	}

	success++;
	printc(green, "Ok", msg);
	return EXIT_SUCCESS;
}

void cleanup() {
	if (str != NULL) {
		free(str);
		str = NULL;
	}
}

void results() {
	printf("\n");
	printf("  -------------\n");

	if (failure ==! 0) {
		prints(green, "Passing", success);
		prints(magenta, "Failed", failure);
		prints(blue, "Total", success + failure);
		return;
	}

	prints(green, "Total", success + failure);
}

int main(int argc, char *argv[]) {
	int res, max, flag = 0;
	const char *longstr, *istr;

	// 1.
	res = asprintf(&str, "foo");
	test(res != -1 && str != NULL, "Basic memory allocation");

	// 2.
	res = asprintf(&str, "%s", "");
	test(res > 0 || strcmp(str, "") == 0, "Empty string as input");

	// 3.
	res = asprintf(&str, "%d %s", 10, "Test");
	test(res > 0 && strcmp(str, "10 Test") == 0,
		"String formatting variations");

	// 4.
	res = asprintf(&str, "Special chars: %% \\");
	test(res > 0 && strcmp(str, "Special chars: % \\") == 0,
		"Special characters in format string");

	// 6.
	res = asprintf(&str, "%d %d", INT_MIN, INT_MAX);
	test(res > 0, "Boundary cases for integers");

	// 7.
	res = asprintf(&str, "%.3f %.3f", (double)FLT_MIN, (double)FLT_MAX);
	test(res > 0, "Corner cases for floating point numbers");

	// 8.
	res = asprintf(&str, "%+d %0#x", 123, 255);
	test(res > 0 && strcmp(str, "+123 0xff") == 0,
		"Combination of flags and specifiers");

	// 9.
	res = asprintf(&str, "");
	test(res == 0 && strcmp(str, "") == 0, "Empty format string");

	// 10.
	res = asprintf(&str, "%s", "St.ing");
	test(res > 0 && (int)strlen(str) == res && str[res] == '\0',
		"Null termination and string length");

	// 11.
	res = asprintf(&str, "3 args: %d, %f, %s", 20, 3.14, "bar");
	test(res != -1 && str != NULL, "Multiple argument counts");

	// 12.
	char *l_str = malloc(INT_MAX);
	memset(l_str, 'A', INT_MAX - 1);
	l_str[INT_MAX - 1] = '\0';
	res = asprintf(&str, "Large string: %s", l_str);
	test(res == -1 && str == NULL, "Creating a very large string");
	free(l_str);

	// 13.
	res = asprintf(&str, "%*s", 10000000, "large str fmt");
	test(res != -1 || str == NULL, "Allocate specified large width");

	// 14.
	res = asprintf(&str, "Escape chars: %d\t%d\n", 10, 20);
	test(res > 0 && strcmp(str, "Escape chars: 10\t20\n") == 0,
		"Escape characters");

	// 15.
	res = asprintf(&str, "%s", "(null)");
	test(res > 0 && strcmp(str, "(null)") == 0, "Null pointers formatting");

	// 16.
	res = asprintf(&str, "null");
	test(res != -1, "With null argument");

	// 17.
	res = asprintf(&str, "Pointer value: %p", (void *)NULL);
	test(res > 0, "Pointer formatting corner cases");

	// 18.
	char *ptr = NULL;
	res = asprintf(&ptr, "This is a test: %d", 42);
	test(res != -1 && ptr != NULL, "Null output string pointer");

	// 19.
	//  a. Longer than given width
	longstr = "This is a long string";
	res = asprintf(&str, "%1000s", longstr);
	flag |= (res > 0 || strcmp(str, longstr) == 0);

	//  b. Given width smaller than string length
	max = 10; istr = "This string is longer than allowed length";
	res = asprintf(&str, "%.*s", max, istr);
	flag |= (res == max || strncmp(str, istr, max) == 0 ||
		str[max] == '\0');

	//  c. String longer string than the given width (max)
	max = 1000; longstr = "This is a long string";
	res = asprintf(&str, "%.*s", max, longstr);
	flag |= (res > 0 || (size_t)res <= max ||
		strcmp(str, longstr) == 0);
	test(flag, "String truncation scenarios");

	// 20.
	wchar_t wc = L'\u00A9';
	res = asprintf(&str, "Wide char: %lc %s", wc, "A");
	test(res > 0 && strcmp(str, "Wide char: © A"),
		"Regular and wide characters");

	// 21.
	setlocale(LC_ALL, "en_US.UTF-8");
	res = asprintf(&str, "%s %lc", "日本語", L'\u00A9');
	test(res > 0 && strcmp(str, "日本語 ©") == 0,
		"Locale and multibyte characters");
	setlocale(LC_ALL, "");

	// 22.
	res = asprintf(&str, "%04d %s %#x", 5, "Test", 255);
	test(res > 0 && strcmp(str, "0005 Test 0xff") == 0,
		"Combining multiple specifiers");

	// 23.
	res = asprintf(&str, "res: %d", 42);
	test(res != -1 && strcmp(str, "res: 42") == 0,
		"Integer and string substitution");

	// 24.
	res = asprintf(&str, "val: %.2f", 3.14159);
	test(res != -1 && strcmp(str, "val: 3.14") == 0,
		"Using %f float substitution specifier");

	// 25.
	const double flt = 3.14159;
	const char *exp = "Hex: 0x1.921f9f01b866ep+1";
	res = asprintf(&str, "Hex: %a", flt);
	test(res == strlen(exp) && strcmp(str, exp) == 0,
		"Using %a hexadecimal floating-point specifier");

	// 26.
	res = asprintf(&str, "%e", 12345.6789);
	test(res > 0 && strcmp(str, "1.234568e+04") == 0,
		"Using %e scientific notation specifier");

	// 27.
	res = asprintf(&str, "%*.*f", 8, 2, 1234.5678);
	test(res > 0 && strcmp(str, " 1234.57") == 0,
		"Using * as width and precision specifier");

	// 28.
	res = asprintf(&str, "Quoted: \"%s\" Escaped quotes: \'%c\'",
		"example", '"');
	test(res > 0 && strcmp(str,
		"Quoted: \"example\" Escaped quotes: '\"'") == 0,
		"Quoted strings with different escape characters");

	// 29.
	int chars = 0;
	res = asprintf(&str, "Chars written: %n", &chars);
	test(res > 0 && chars == res,
		"Using %n to get the number of characters written");

	// 30.
	count(16, "this is a %s", "string");
	count(9, "%d + %d = %d", 1, 1, 2);
	count(56, "bradley likes %s, %s, %s, and %s",
		"kinkajous", "bananas", "monkeys", "the beach");

	cleanup();
	results();
}
