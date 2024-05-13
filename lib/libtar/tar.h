/*
 * tar.h
 * Tar archive header
 * Based on IBM AIX 7.2 tar.h specifications
 *
 * Copyright (c) 2021 Ariston Lorenzo.
 *
 */

#ifndef __TAR__
#define __TAR__

#include "../LibC/errno.h"
#include "../LibC/stdarg.h"
#include "../LibC/stdio.h"
#include "../LibC/stdlib.h"
#include "../LibC/string.h"
#include "../LibC/time.h"

#include "../LibC/unistd.h"

#define BLOCKSECTOR 512

#define REGULAR 0
#define NORMAL '0'
#define HARDLINK '1'
#define SYMLINK '2'
#define CHAR '3'
#define BLOCK '4'
#define DIRECTORY '5'
#define FIFO '6'
#define CONTIGUOUS '7'

struct ustar {
    char name[100];
    char mode[8];
    char uid[8];
    char gid[8];
    char size[12];  // octal base
    char mtime[12]; // octal base
    char cksum[8];
    char typeflag[1];
    char linkname[100];
    char magic[6];
    char version[3];
    char uname[32];
    char gname[32];
    char devmajor[8];
    char devminor[8];
    char prefix[155];
};

#endif
