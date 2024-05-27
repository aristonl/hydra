#include <stdio.h>
#include <stdlib.h>

static void
usage(void)
{
	printf("Usage: nmake [OPTIONS] [TARGET]\n");
	printf("OPTIONS:\n");
	printf("	-h - Print this message\n");
	printf("	-v - Print version and build information\n");
}

static void
version(void)
{
	printf("nmake version %s\n", NMAKE_VERSION);
	printf("Copyright (c) 2024 N11 Software. All rights reserved.\n\n");
	printf("Built by: %s\n", BUILD_HOST);
	printf("Build date: %s\n", BUILD_DATE);
}

static int
getopt(argc, argv, arg)
	int argc;
	char* argv[];
	const char* arg;
{
	int i;

	for (i = 1; i < argc; i++) {
		if (strcmp(argv[i], arg) == 0) {
			return 1;
		}
	}

	return 0;
}

int
main(argc, argv)
	int argc;
	char* argv[];
{
	if (getopt(argc, argv, "-h")) {
		usage();
		return 0;
	}

	if (getopt(argc, argv, "-v")) {
		version();
		return 0;
	}

	if (argc < 2) {
		usage();
		return 1;
	}

	return 0;
}
