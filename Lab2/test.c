#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tools.h"
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
	print("enter");
	   // h e l l o   w o r l d     f     u  c  k
	   // 0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22
	char* stra = "you're all a bunch of rotten mouth whore fucks";
	char* str = malloc(sizeof(char)*strlen(stra));
	strcpy(str, stra);
	lineWrap(str, 8);
	printf("%s\n", str);

	// int charLeft = 16;

	
}

void lineWrap(char* str, int wrapLen) {
	int lastWrapIdx = 0;
	int k;
	int wrapIdx = 0;

	for(int i = 0; str[i] != '\0'; i++, wrapIdx++) {
		print("enter %d", i);
		if (wrapIdx == wrapLen) {
			// chunk of text over line length
			if (str[i] != ' ') {
				// backtrack to last white space
				for (k = i; k > 0 && str[k] != ' '; k--) {
					continue;
				}
				i = k + 1;
				wrapIdx = 0; 
				print("wrapping at %d", k);
				str[k]='\n';

			} else {
				print("wrapping at %d", i);
				str[i]='\n';
				i++;
				wrapIdx = 0;
			}
		}
	}
}


	// while (charLeft > 0) {
	// 	// printf("--------------------------------------");
	// 	// print("%c", str[base]);
	// 	while(str[base] == ' '){
	// 		base++;
	// 		charLeft--;
	// 		print("base: %d", base);
	// 	}

	// 	while (str[spc] != ' ' && spc < charLeft) {
	// 		spc++;
	// 		print("spc: %d", spc);
	// 	}

	// 	if ((spc - base) <= 8) {
	// 		print("Diff: %d  lastEnd: %d  spc: %d  base: %d", (spc - base), lastEnd, spc, base);

	// 		lastEnd = spc - 1;
	// 		spc++;
	// 	} else {
	// 		print("base: %d  lastEnd: %d  charLeft: %d  spc: %d", base, lastEnd, charLeft, spc);

	// 		printf("%.*s\n", lastEnd - base + 1, &str[base]);
	// 		base = lastEnd + 1;
	// 		lastEnd = spc - 1;
	// 		charLeft = charLeft - (lastEnd - base + 1);
	// 		spc++;
	// 		print("base: %d  lastEnd: %d  charLeft: %d  spc: %d", base, lastEnd, charLeft, spc);
