# N11 Software Coding Style
This document contains information on the general coding guidelines are for N11 Software. 

## Indentation

### Tabs vs. Spaces
Tabs should be used in place of spaces when indenting code.

### Indent Size
Indent size is 4 spaces.

```cpp
int foo() {
	int bar = 0;
	int baz = 0;
	int qux = 0;
}
```

### Bracket Placing
Brackets should be on the same line as its parent statement
```cpp
if (foo == true) {
	// code.
}
```

### Line Length
80 is the recommended maximum for line length. What do we mean by 'recommended'?
If your statement just happens to go slightly over the line (anymore from around
81-90) is plausible considering indentation and the fact that it's just slightly
over the limit so we don't really care. If you do go over this limit, it's a
good time to check to see if something is over-engineered. In the case of emails,
commit descriptions, etc., these should all follow the 80 line limit since it's
better for our (by default) 80 x 60 terminal windows.

## Notation

### Hungarian Notation
"Hungarian Notation" was a common practice back in 90's C/C++ style for Windows
and is still used today in some projects. We have adopted use of it in N11.
This part isn't a 'requirement' since in some use cases, it really doesn't
matter or it just doesn't look good in which we'll break this section. We still
encourage you to do this as much as you can but there isn't usually any need
to.

| Prefix | Description   |
|--------|---------------|
| b      | Boolean       |
| c      | Counter       |
| C		 | Class		 |
| g_     | Global        |
| i      | Integers      |
| m_     | Class members |
| s_	 | Static		 |

### Naming Style
We use CamelCase for everything (file names, functions, etc..). Capitalize the
first letter in classes and structs. Lowercase the first letter in a variable
or function name.

```cpp
struct Data
size_t bufferSize;
class CHTMLDocument;
String mimeType();
```

## Comments

### Copyright Header
```cpp
//========= Copyright N11 Software, All rights reserved. ============//
//
// Purpose: ---
//
//===================================================================//
```

### Declaration Header
```cpp
//===================================================================//
//
//===================================================================//
```

### #endif
All `#endif` statements must have a comment to what `#if`/`#ifndef`; it belongs to.
