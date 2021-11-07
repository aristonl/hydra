#ifndef TextProtocol

struct TextProtocol {
	unsigned long long (*unused1)(struct TextProtocol *, bool);

	unsigned long long (*OutputString)(
		struct TextProtocol *self,
		unsigned short int *string);

	unsigned long long (*unused2)(
		struct TextProtocol *,
		unsigned short int *);
	unsigned long long (*unused3)(
		struct TextProtocol *,
		unsigned long long, unsigned long long *, unsigned long long *);
	unsigned long long (*unused4)(
		struct TextProtocol *,
		unsigned long long);
	unsigned long long (*unused5)(
		struct TextProtocol *,
		unsigned long long);

	unsigned long long (*ClearScreen)(
		struct TextProtocol *self);

	unsigned long long (*unused6)(
		struct TextProtocol *,
		unsigned long long, unsigned long long);
	unsigned long long (*unused7)(
		struct TextProtocol *,
		bool);

	void *unused8;
};

#endif