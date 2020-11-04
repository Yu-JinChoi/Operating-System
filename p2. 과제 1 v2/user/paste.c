#define _GNU_SOURCE
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <signal.h>
#include <errno.h>
#include "kboard.h"
#define MAX_CLIP 6

int main()
{
	int *ring = malloc(sizeof(int)*MAX_CLIP);
	int n;

	n = kboard_paste(ring);

	if(n == -1) {
		printf("Fail: KBoard is empty\n");
	}
	else if(n == 0) {
		printf("Paste success: %d\n", *ring);
	}
}
