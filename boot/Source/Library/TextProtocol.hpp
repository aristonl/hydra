#ifndef TextProtocol

struct TextProtocol {
	void* Reset;
	unsigned long long (*Print)(struct TextProtocol* self, unsigned short int* string);
	void* TestString, *QueryMode, *SetMode, *SetAttribute;
	unsigned long long (*Clear)(struct TextProtocol *self);
	void* SetCursorPosition, *EnableCursor, *Mode;
};

#endif