

#include <stdio.h>

#define DEBUG

#ifdef DEBUG
// debug variant

#define print(fmt, args...) {\
	char _macro_fmt[100]; \
	snprintf(_macro_fmt, 100, fmt, ##args);\
	printf("%s | %s %s:%d\n", _macro_fmt, __func__, __FILE__, __LINE__);\
}

#else
// prod variant

#define print(fmt, args...)

#endif

int main() {
	// print("here");
	print("swag %d", 100);
	print("hello");
}