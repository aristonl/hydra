#include <log.h>

#include <COM.hpp>

void prInfo(const char* subsystem, const char* message) {
	kprintf("\r\e\033[1;32m[INFO] %s: %s\e[0m\n\r", subsystem, message);
}

void prErr(const char* subsystem, const char* message) {
	kprintf("\r\e\033[1;31m[ERROR] %s: %s\e[0m\n\r", subsystem, message);
}

void prWarn(const char* subsystem, const char* message) {
	kprintf("\r\e\033[1;33m[WARNING] %s: %s\e[0m\n\r", subsystem, message);
}

void prDebug(const char* subsystem, const char* message) {
	kprintf("\r\e\033[1;34m[DEBUG] %s: %s\e[0m\n\r", subsystem, message);
}
