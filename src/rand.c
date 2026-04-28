#include "cality.h"
#include <string.h>

static thread_local uvlong q_seed = 0x1234567890ABCDEFULL;
static thread_local uvlong r_seed = 0xFEDCBA0987654321ULL;

void seedfeed(arrst seed) {
	if (seed.len >= sizeof(uvlong)) {
		memcpy(&q_seed, seed.x, sizeof(uvlong));
		r_seed = q_seed ^ 0x9E3779B97F4A7C15ULL;
	} else if (seed.len > 0) {
		memcpy(&q_seed, seed.x, seed.len);
		r_seed = q_seed ^ 0x9E3779B97F4A7C15ULL;
	}
}

// Simplified w1rand (wyrand) for testing
uvlong qrand(void) {
	q_seed += 0xa0761d6478bd642fULL;
	uvlong see = q_seed ^ 0xe7037ed1a0b428dbULL;
	
#if defined(__SIZEOF_INT128__)
	unsigned __int128 p = (unsigned __int128)q_seed * see;
	return (uvlong)(p >> 64) ^ (uvlong)p;
#elif defined(_MSC_VER) && defined(_WIN64)
	uvlong h, l;
	l = _umul128(q_seed, see, &h);
	return h ^ l;
#else
	// Fallback 64-bit mix
	uvlong z = (q_seed ^ (see >> 32)) * 0x4f1bbcdcbfa54c43ULL;
	return z ^ (z >> 32);
#endif
}

// Minimal placeholder for chacha8-based rand. Using SplitMix64 currently.
uvlong rand(void) {
	uvlong z = (r_seed += 0x9e3779b97f4a7c15ULL);
	z = (z ^ (z >> 30)) * 0xbf58476d1ce4e5b9ULL;
	z = (z ^ (z >> 27)) * 0x94d049bb133111ebULL;
	return z ^ (z >> 31);
}
