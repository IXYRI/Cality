#include "cality.h"
#include <stdio.h>

void               exit(int status);

thread_local char *error_msg = null;

bool               err(void) { return error_msg != null; }

char              *errmsg(char *setmsg) {
	if (error_msg != null && setmsg != null) {
		fprintf(stderr, "Fatal error: Double fault.\nOld: %s\nNew: %s\n", error_msg, setmsg);
		exit(1);
	}
	char *oldmsg = error_msg;
	error_msg    = setmsg;
	return oldmsg;
}

void efail(void) {
	if (error_msg) {
		fprintf(stderr, "Fatal error: %s\n", error_msg);
		exit(1);
	}
}
