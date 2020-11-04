#define _GNU_SOURCE
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <signal.h>
#include <errno.h>
#include "kboard.h"

int main(int argc, char *argv[])
{
	int i, n;

	for(int j=1; j<argc; j++) {
		n = atoi(argv[j]);
		i = kboard_copy(n);
		
		if(i == -1 || i == -2)
		        printf("Fail: KBoard is full or invaild clip\n");
	        else
			printf("Copy success: %d\n", n);

	}

}
