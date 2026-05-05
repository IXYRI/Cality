#include "../src/cality.h"
#include <stdio.h>
#include <string.h>

int main() {
	printf("Running hash tests...\n");
	
	char *msg = "hello world";
	arrst data = mkarrst(strlen(msg), msg);
	
	uvlong h1 = hash(data);
	uvlong h2 = qhash(data);
	utlong ch = chash(data);
	
	printf("hash:  %llx\n", h1);
	printf("qhash: %llx\n", h2);
	printf("chash: %llx %llx %llx %llx\n", ch.b, ch.d, ch.u, ch.t);
	
	// Test consistency
	assert(hash(data) == h1);
	assert(qhash(data) == h2);
	
	// Slight change in msg
	char *msg2 = "hello workd";
	arrst data2 = mkarrst(strlen(msg2), msg2);
	assert(hash(data2) != h1);
	
	printf("Hash tests passed.\n");
	return 0;
}
