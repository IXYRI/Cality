#include "../src/cality.h"
#include <stdio.h>

int main() {
	printf("Running rand tests...\n");

	uvlong val  = 0xdeadbeefcafebabeull;
	arrst  seed = tobytes(val);
	seedfeed(seed);

	uvlong u1 = qrand();
	uvlong u2 = qrand();
	printf("qrand1: %llx\n", u1);
	printf("qrand2: %llx\n", u2);
	assert(u1 != u2);

	uvlong r1 = rand();
	uvlong r2 = rand();
	printf("rand1:  %llx\n", r1);
	printf("rand2:  %llx\n", r2);
	assert(r1 != r2);

	printf("Rand tests passed.\n");
	return 0;
}
