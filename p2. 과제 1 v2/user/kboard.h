#include <stdio.h>

long kboard_copy(int clip);
int kboard_paste(int *clip);

long kboard_copy(int clip) {
	int i =  syscall(335, clip);

	return i;
}

int kboard_paste(int *clip) {
	return syscall(336, clip);
}
