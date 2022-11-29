#include <cpuid.h>

#include <COM.hpp>
#include <stddef.h>

#define cpuid(in, a, b, c, d) __asm__("cpuid": "=a" (a), "=b" (b), "=c" (c), "=d" (d) : "a" (in));

int detect_cpu(void) {
	unsigned long ebx, unused;
	cpuid(0, unused, ebx, unused, unused);
	switch(ebx) {
		case 0x756e6547: /* Intel Magic Code */
		kprintf("\r\e[92m[INFO] Intel CPU Detected\n");
		break;
		case 0x68747541: /* AMD Magic Code */
		kprintf("\r\e[92m[INFO] AMD CPU Detected\n");
		break;
		default:
		kprintf("\r\e[31m[ERROR] Unknown x86 CPU Detected\n");
		break;
	}
	return 0;
}
