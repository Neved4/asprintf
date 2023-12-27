#ifndef HAVE_ASPRINTF
#define HAVE_ASPRINTF 1

#include <stdarg.h>

int vasprintf(char **strp, const char *fmt, va_list ap);

int asprintf(char **s, const char *fmt, ...);

#endif
