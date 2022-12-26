#pragma once

#include <COM.hpp>
#include <stdarg.h>

void prInfo(const char* subsystem, const char* message, ...);
void prErr(const char* subsystem, const char* message, ...);
void prWarn(const char* subsystem, const char* message, ...);
void prDebug(const char* subsystem, const char* message, ...);
