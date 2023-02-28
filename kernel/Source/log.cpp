//========= Copyright N11 Software, All rights reserved. ============//
//
// File: log.cpp
// Purpose: Message logging functions
// Maintainer: aristonl
//
//===================================================================//

#include <log.h>

#include <COM.hpp>
#include <stdarg.h>

void prInfo(const char* subsystem, const char* message, ...) {
	va_list args;
	va_start(args, message);

	kprintf("\r\e\033[32m[INFO] \033[0;33m%s\033[0m: ", subsystem);
	for (const char* p = message; *p; ++p) {
		if (*p == '%') {
			++p;
			switch (*p) {
			case 'd':
				kprintf("%d", va_arg(args, int));
				break;
			case 's' :
				kprintf("%s", va_arg(args, char*));
				break;
			}
		} else {
			kputchar(*p);
		}
	}
	kprintf("\e[0m\n\r");

	va_end(args);
}

void prErr(const char* subsystem, const char* message, ...) {
	va_list args;
	va_start(args, message);

	kprintf("\r\e\033[31m[ERROR] \033[0;33m%s\033[0m: ", subsystem);
	for (const char* p = message; *p; ++p) {
		if (*p == '%') {
			++p;
			switch (*p) {
			case 'd':
				kprintf("%d", va_arg(args, int));
				break;
			case 's':
				kprintf("%s", va_arg(args, char*));
				break;
			case 'X':
				kprintf("%X", va_arg(args, int));
				break;
			}
		} else {
			kputchar(*p);
		}
	}
	kprintf("\e[0m\n\r");

	va_end(args);
}

void prWarn(const char* subsystem, const char* message, ...) {
	va_list args;
	va_start(args, message);

	kprintf("\r\e\033[33m[WARNING] \033[0;33m%s\033[0m: ", subsystem);
	for (const char* p = message; *p; ++p) {
		if (*p == '%') {
			++p;
			switch (*p) {
			case 'd':
				kprintf("%d", va_arg(args, int));
				break;
			case 's':
				kprintf("%s", va_arg(args, char*));
				break;
			}
		} else {
			kputchar(*p);
		}
	}
	kprintf("\e[0m\n\r");

	va_end(args);
}

void prDebug(const char* subsystem, const char* message, ...) {
	va_list args;
	va_start(args, message);

	kprintf("\r\e\033[34m[DEBUG] \033[0;33m%s\033[0m: ", subsystem);
	for (const char* p = message; *p; ++p) {
		if (*p == '%') {
			++p;
			switch (*p) {
			case 'd':
				kprintf("%d", va_arg(args, int));
				break;
			case 's':
				kprintf("%s", va_arg(args, char*));
				break;
			}
		} else {
			kputchar(*p);
		}
	}
	kprintf("\e[0m\n\r");

	va_end(args);
}
