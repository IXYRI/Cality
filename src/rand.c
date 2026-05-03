#include "cality.h"
#include <string.h>

static thread_local uvlong q_seed            = 0x1234567890abcdefull;
static thread_local uint   chacha_state [16] = {
  0x61707865, 0x3320646e, 0x79622d32, 0x6b206574,   // "expand 32-byte k"
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0                // key, counter, nonce
};
static thread_local uvlong chacha_out [8];
static thread_local int    chacha_idx = 8;

void                       seedfeed(arrst seed) {
	if (seed.len >= sizeof(uvlong)) memcpy(&q_seed, seed.x, sizeof(uvlong));
	else if (seed.len > 0) memcpy(&q_seed, seed.x, seed.len);

	// Feed chacha8 seed
	for (int i = 0; i < 8; i++) chacha_state [4 + i] = ( uint ) (q_seed >> (i % 2 == 0 ? 0 : 32)) ^ (i * 0x9e3779b9);
	chacha_idx = 8;
}

// Simplified w1rand (wyrand) for testing
uvlong qrand(void) {
	q_seed     += 0xa0761d6478bd642full;
	uvlong see  = q_seed ^ 0xe7037ed1a0b428dbull;
	urlong p    = wmul_vl(q_seed, see);
	return p.l ^ p.h;
}

// ChaCha8 core block generator
static void chacha8_block(void) {
	uint x [16];
	memcpy(x, chacha_state, 64);

	for (int i = 0; i < 4; i++) {
#define QR(a, b, c, d)       \
	x [a] += x [b];          \
	x [d] ^= x [a];          \
	x [d]  = rol(x [d], 16); \
	x [c] += x [d];          \
	x [b] ^= x [c];          \
	x [b]  = rol(x [b], 12); \
	x [a] += x [b];          \
	x [d] ^= x [a];          \
	x [d]  = rol(x [d], 8);  \
	x [c] += x [d];          \
	x [b] ^= x [c];          \
	x [b]  = rol(x [b], 7)
		QR(0, 4, 8, 12);
		QR(1, 5, 9, 13);
		QR(2, 6, 10, 14);
		QR(3, 7, 11, 15);
		QR(0, 5, 10, 15);
		QR(1, 6, 11, 12);
		QR(2, 7, 8, 13);
		QR(3, 4, 9, 14);
#undef QR
	}

	for (int i = 0; i < 16; i++) (( uint * ) chacha_out) [i] = x [i] + chacha_state [i];
	chacha_state [12]++;   // Increment counter
}

uvlong rand(void) {
	if (chacha_idx >= 8) {
		chacha8_block();
		chacha_idx = 0;
	}
	return chacha_out [chacha_idx++];
}
