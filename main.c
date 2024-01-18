#include <stdio.h>
#include <stdlib.h>

#include "asprintf.h"

int main(int argc, char *argv[]) {
	char *str;

	asprintf(&str, "Hello, %s!", "world");
	printf("%s\n", str);
	free(str);
}
