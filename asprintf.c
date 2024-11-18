#include <stdio.h>
#include <stdlib.h>
#include "asprintf.h"

int vasprintf(char **strp, const char *fmt, va_list ap) {
	int size, res;
	va_list cp;

	va_copy(cp, ap);
	size = vsnprintf(NULL, 0, fmt, cp);
	va_end(cp);

	if (size < 0) {
		return -1;
	}

	*strp = malloc(size + 1);
	if (*strp == NULL) {
		return -1;
	}

	res = vsnprintf(*strp, size + 1, fmt, ap);
	if (res < 0) {
		free(*strp);
		return -1;
	}

	return res;
}

int asprintf(char **s, const char *fmt, ...) {
	int ret;

	va_list ap;
	va_start(ap, fmt);
	ret = vasprintf(s, fmt, ap);
	va_end(ap);

	return ret;
}
