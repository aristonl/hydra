/*
 * ustar.h
 * Implementation of the USTAR filesystem. Also used in 'tar' files.
 *
 * Copyright (c) 2021 Ariston Lorenzo.
 *
 */

#ifndef __USTAR__
#define __USTAR__

#include <LibC/errno.h>
#include <LibC/stdarg.h>
#include <LibC/stdio.h>
#include <LibC/stdlib.h>
#include <LibC/string.h>
#include <LibC/time.h>

#include <LibC/unistd.h>

#define BLOCKSECTOR 512

#define REGULAR 	0
#define NORMAL 		'0'
#define HARDLINK 	'1'
#define SYMLINK 	'2'
#define CHAR 		'3'
#define BLOCK		'4'
#define DIRECTORY	'5'
#define FIFO		'6'
#define CONTIGUOUS	'7'

struct ustar {
	char name[100];
	char mode[8];
	char owner_uid[8];
	char group_uid[8];
	char size[12]		// octal base
	char mtime[12];		// octal base
	char checksum[8];
	char link;		// link indicator
	char link_name[100];
	char type;
	char ustar[8];
	char owner[32];
	char group[32];
	char major[8];
	char minor[8];
	char prefix[155];
};

#endif
