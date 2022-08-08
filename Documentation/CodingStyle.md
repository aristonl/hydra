# Echo Coding Style
This document contains the coding style used in the Echo project. All new code should follow this styleguide.

## Names
We use PascalCase in a class, struct, or namespace (also includes capitalising acronyms), and snake_case for variable/function names.
<br>
<br>
Right:
```cpp
class BOB {
	struct FileDescriptor;
	size_t buffer_size;
}
```
Wrong:
```cpp
class bob {
	struct Filedescriptor;
	size_t BufferSize;
}
```

Use full words as much as possible, except where an abbreviation is more understandable and canonical.
<br>
<br>
Right:
```cpp
size_t character_size;
size_t length;
short tab_index; // More canonical.
```
Wrong:
```cpp
size_t char_size;
size_t len;
short tabulation_index; // Just no.
```
We use `#pragma once` to prevent multiple inclusion of header files rather than `#ifndef`/`#define`/`#endif`.
<br>
<br>
Right:
```cpp
#pragma once
```
Wrong:
```cpp
#ifndef INFERNO_FILE_DESCRIPTOR_H
#define INFERNO_FILE_DESCRIPTOR_H
#endif
```

## Indentation
We use 4 spaces for indentation since it's not too small as 2 spaces and not too big as 8 spaces. We also don't accept the use of spaces rather than tabs.
<br>
<br>
Right:
```cpp
class BOB {
	struct FileDescriptor;
	size_t buffer_size;
}
```
Wrong:
```cpp
class BOB {
  struct FileDescriptor;
  size_t buffer_size;
}
```
Contents of a `namespace` block should not be indented unless it is a nested `namespace` block.
<br>
<br>
Right:
```cpp
namespace BOB {
class FileDescriptor;
size_t buffer_size;
}
```
Wrong:
```cpp
namespace BOB {
	class FileDescriptor;
	size_t buffer_size;
}
```

## Spacing
Do not place spaces between unary operators.
<br>
<br>
Right:
```cpp
i++;
```
Wrong:
```cpp
i ++;
```

Place spaces between binary and ternary operators.
<br>
<br>
Right:
```cpp
i = i + 1;
```
Wrong:
```cpp
i = i+1;
```

## Line Breaking
Each statement gets its own line.
<br>
<br>
Right:
```cpp
x++;
y++;
if (condition)
    doIt();
```
Wrong:
```cpp
x++; y++;
if (condition) doIt();
```
The `else` statement should be on the same line as the closing brace of the `if` statement (if present). If a closing brace is missing, the `else` statement must be aligned with the `if` statement.
<br>
<br>
Right:
```cpp
if (condition)
	doIt();
else
	doThat();

if (condition) {
	doIt();
} else {
	doThat();
}
```
Wrong:
```cpp
if (condition) doIt(); else doThat();

if (condition) {
	doIt();
} 
else {
	doThat();
}
```

## Braces
Braces will be placed on the same line as the statement they are associated with.
<br>
<br>
Right:
```cpp
int main() {
	return 0;
}
```
Wrong:
```cpp
int main()
{
	return 0;
}
```