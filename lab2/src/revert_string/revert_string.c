#include <stdio.h>
#include <string.h>
#include "revert_string.h"


void swap(char *left, char *right)
{
	*left  ^= *right;
	*right ^= *left;
	*left  ^= *right;
}

void RevertString(char *str)
{
	if (str == NULL) {
		printf("RevertString: str is NULL");
		return;
	}
	
	size_t n = strlen(str);
	for (size_t i = 0; i < n / 2; i++) {
		swap(str + i, str + n - i - 1);
	}
}

