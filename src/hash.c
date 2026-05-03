#include "cality.h"
#include <string.h>

#define SIPROUND(v0, v1, v2, v3) \
	do {                         \
		v0 += v1;                \
		v1  = rol_vl(v1, 13);    \
		v1 ^= v0;                \
		v0  = rol_vl(v0, 32);    \
		v2 += v3;                \
		v3  = rol_vl(v3, 16);    \
		v3 ^= v2;                \
		v0 += v3;                \
		v3  = rol_vl(v3, 21);    \
		v3 ^= v0;                \
		v2 += v1;                \
		v1  = rol_vl(v1, 17);    \
		v1 ^= v2;                \
		v2  = rol_vl(v2, 32);    \
	}                            \
	while (0)

// SipHash-2-4
uvlong hash(arrst data) {
	uvlong k0 = 0x0706050403020100ull, k1 = 0x0f0e0d0c0b0a0908ull;
	uvlong v0    = k0 ^ 0x736f6d6570736575ull;
	uvlong v1    = k1 ^ 0x646f72616e646f6dull;
	uvlong v2    = k0 ^ 0x6c7967656e657261ull;
	uvlong v3    = k1 ^ 0x7465646279746573ull;

	uchar *in    = ( uchar * ) data.x;
	uvlong inlen = data.len;
	int    left  = inlen & 7;
	uchar *end   = in + inlen - left;

	for (; in != end; in += 8) {
		uvlong m;
		memcpy(&m, in, 8);
		v3 ^= m;
		SIPROUND(v0, v1, v2, v3);
		SIPROUND(v0, v1, v2, v3);
		v0 ^= m;
	}

	uvlong b = (( uvlong ) inlen) << 56;
	switch (left) {
	case 7 : b |= (( uvlong ) in [6]) << 48;
	case 6 : b |= (( uvlong ) in [5]) << 40;
	case 5 : b |= (( uvlong ) in [4]) << 32;
	case 4 : b |= (( uvlong ) in [3]) << 24;
	case 3 : b |= (( uvlong ) in [2]) << 16;
	case 2 : b |= (( uvlong ) in [1]) << 8;
	case 1 : b |= (( uvlong ) in [0]); break;
	case 0 : break;
	}

	v3 ^= b;
	SIPROUND(v0, v1, v2, v3);
	SIPROUND(v0, v1, v2, v3);
	v0 ^= b;
	v2 ^= 0xff;
	SIPROUND(v0, v1, v2, v3);
	SIPROUND(v0, v1, v2, v3);
	SIPROUND(v0, v1, v2, v3);
	SIPROUND(v0, v1, v2, v3);

	return v0 ^ v1 ^ v2 ^ v3;
}

static uvlong const PRIME64_1 = 0x9e3779b185ebca87ull;
static uvlong const PRIME64_2 = 0xc2b2ae3d27d4eb4full;
static uvlong const PRIME64_3 = 0x165667b19e3779f9ull;
static uvlong const PRIME64_4 = 0x85ebca77c2b2ae63ull;
static uvlong const PRIME64_5 = 0x27d4eb2f165667c5ull;

static uvlong inline xx64_round(uvlong acc, uvlong input) {
	acc += input * PRIME64_2;
	acc  = rol_vl(acc, 31);
	acc *= PRIME64_1;
	return acc;
}

static uvlong inline xx64_merge(uvlong acc, uvlong val) {
	acc ^= xx64_round(0, val);
	acc  = acc * PRIME64_1 + PRIME64_4;
	return acc;
}

// XXHash64
uvlong qhash(arrst data) {
	uchar *p    = ( uchar * ) data.x;
	uchar *bEnd = p + data.len;
	uvlong h64;

	if (data.len >= 32) {
		uchar *limit = bEnd - 32;
		uvlong v1    = PRIME64_1 + PRIME64_2;
		uvlong v2    = PRIME64_2;
		uvlong v3    = 0;
		uvlong v4    = -( long long ) PRIME64_1;

		do {
			uvlong m1, m2, m3, m4;
			memcpy(&m1, p, 8);
			memcpy(&m2, p + 8, 8);
			memcpy(&m3, p + 16, 8);
			memcpy(&m4, p + 24, 8);
			v1  = xx64_round(v1, m1);
			v2  = xx64_round(v2, m2);
			v3  = xx64_round(v3, m3);
			v4  = xx64_round(v4, m4);
			p  += 32;
		}
		while (p <= limit);

		h64 = rol_vl(v1, 1) + rol_vl(v2, 7) + rol_vl(v3, 12) + rol_vl(v4, 18);
		h64 = xx64_merge(h64, v1);
		h64 = xx64_merge(h64, v2);
		h64 = xx64_merge(h64, v3);
		h64 = xx64_merge(h64, v4);
	}
	else { h64 = PRIME64_5; }

	h64 += ( uvlong ) data.len;

	while (p + 8 <= bEnd) {
		uvlong k1;
		memcpy(&k1, p, 8);
		uvlong k1r  = xx64_round(0, k1);
		h64        ^= k1r;
		h64         = rol_vl(h64, 27) * PRIME64_1 + PRIME64_4;
		p          += 8;
	}

	if (p + 4 <= bEnd) {
		uint k1;
		memcpy(&k1, p, 4);
		h64 ^= ( uvlong ) ( k1 ) *PRIME64_1;
		h64  = rol_vl(h64, 23) * PRIME64_2 + PRIME64_3;
		p   += 4;
	}

	while (p < bEnd) {
		h64 ^= (*p) * PRIME64_5;
		h64  = rol_vl(h64, 11) * PRIME64_1;
		p++;
	}

	h64 ^= h64 >> 33;
	h64 *= PRIME64_2;
	h64 ^= h64 >> 29;
	h64 *= PRIME64_3;
	h64 ^= h64 >> 32;

	return h64;
}

#define B3_START 1
#define B3_END   2
#define B3_PAR   4
#define B3_ROOT  8

static uint const b3_iv [8]
  = {0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a, 0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19};

static uchar const b3_msg_sched [7][16] = {
  {0,  1,  2,  3,  4,  5,  6,  7,  8,  9,  10, 11, 12, 13, 14, 15},
  {2,  6,  3,  10, 7,  0,  4,  13, 1,  11, 5,  12, 9,  14, 15, 8 },
  {3,  4,  10, 5,  13, 2,  7,  14, 6,  12, 0,  9,  11, 15, 8,  1 },
  {10, 7,  5,  0,  14, 3,  13, 15, 4,  9,  2,  11, 12, 8,  1,  6 },
  {5,  13, 0,  2,  15, 10, 14, 8,  7,  11, 3,  12, 9,  1,  6,  4 },
  {0,  14, 2,  3,  8,  5,  15, 1,  13, 12, 10, 9,  11, 6,  4,  7 },
  {2,  15, 3,  10, 1,  0,  8,  6,  14, 9,  5,  11, 12, 4,  7,  13}
};

#define B3G(a, b, c, d, x, y) \
	do {                      \
		a = a + b + x;        \
		d = ror(d ^ a, 16);   \
		c = c + d;            \
		b = ror(b ^ c, 12);   \
		a = a + b + y;        \
		d = ror(d ^ a, 8);    \
		c = c + d;            \
		b = ror(b ^ c, 7);    \
	}                         \
	while (0)

static void b3_compress(uint cv [8], uchar const block [64], uint b_len, uvlong counter, uint flags) {
	uint v [16];
	memcpy(v, cv, 32);
	memcpy(v + 8, b3_iv, 32);
	v [12] ^= ( uint ) counter;
	v [13] ^= ( uint ) (counter >> 32);
	v [14] ^= b_len;
	v [15] ^= flags;

	uint m [16];
	memcpy(m, block, 64);

	for (int i = 0; i < 7; i++) {
		uchar const *s = b3_msg_sched [i];
		B3G(v [0], v [4], v [8], v [12], m [s [0]], m [s [1]]);
		B3G(v [1], v [5], v [9], v [13], m [s [2]], m [s [3]]);
		B3G(v [2], v [6], v [10], v [14], m [s [4]], m [s [5]]);
		B3G(v [3], v [7], v [11], v [15], m [s [6]], m [s [7]]);
		B3G(v [0], v [5], v [10], v [15], m [s [8]], m [s [9]]);
		B3G(v [1], v [6], v [11], v [12], m [s [10]], m [s [11]]);
		B3G(v [2], v [7], v [8], v [13], m [s [12]], m [s [13]]);
		B3G(v [3], v [4], v [9], v [14], m [s [14]], m [s [15]]);
	}
	for (int i = 0; i < 8; i++) cv [i] = v [i] ^ v [i + 8];
}

static void b3_chunk(uint cv [8], uchar const *chunk, uvlong chunk_len, uvlong counter, int is_root) {
	memcpy(cv, b3_iv, 32);
	int blocks = ( int ) ((chunk_len + 63) / 64);
	if (blocks == 0) blocks = 1;

	for (int b = 0; b < blocks; b++) {
		uchar block [64] = {0};
		uint  b_len      = 64;
		if (b == blocks - 1) b_len = ( uint ) (chunk_len - b * 64);
		memcpy(block, chunk + b * 64, b_len);

		uint flags = 0;
		if (b == 0) flags |= B3_START;
		if (b == blocks - 1) {
			flags |= B3_END;
			if (is_root) flags |= B3_ROOT;
		}
		b3_compress(cv, block, b_len, counter, flags);
	}
}

static void b3_parent(uint cv [8], uint left [8], uint right [8], int is_root) {
	uchar block [64];
	memcpy(block, left, 32);
	memcpy(block + 32, right, 32);
	memcpy(cv, b3_iv, 32);
	uint flags = B3_PAR;
	if (is_root) flags |= B3_ROOT;
	b3_compress(cv, block, 64, 0, flags);
}

typedef struct {
	uint cv [8];
	int  height;
} b3_node;

utlong chash(arrst msg) {
	uchar *p      = ( uchar * ) msg.x;
	uvlong len    = msg.len;

	uvlong chunks = (len + 1023) / 1024;
	if (chunks == 0) chunks = 1;

	b3_node stack [64];
	int     stack_len = 0;

	for (uvlong c = 0; c < chunks; c++) {
		uvlong chunk_len = 1024;
		if (c == chunks - 1) chunk_len = len - c * 1024;

		uint cv [8];
		b3_chunk(cv, p + c * 1024, chunk_len, c, chunks == 1);

		int height = 0;
		while (stack_len > 0 && stack [stack_len - 1].height == height) {
			uint parent [8];
			b3_parent(parent, stack [stack_len - 1].cv, cv, 0);
			memcpy(cv, parent, 32);
			stack_len--;
			height++;
		}
		memcpy(stack [stack_len].cv, cv, 32);
		stack [stack_len].height = height;
		stack_len++;
	}

	uint cv [8];
	stack_len--;
	memcpy(cv, stack [stack_len].cv, 32);
	while (stack_len > 0) {
		stack_len--;
		uint parent [8];
		b3_parent(parent, stack [stack_len].cv, cv, stack_len == 0);
		memcpy(cv, parent, 32);
	}

	utlong out;
	memcpy(&out, cv, 32);
	return out;
}
