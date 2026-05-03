#pragma once
#include "datom.h"

// Clang & GCC specific implementations using __builtin

int inline clz(uint x) { return x ? __builtin_clz(x) : 32; }

int inline ctz(uint x) { return x ? __builtin_ctz(x) : 32; }

int inline popcnt(uint x) { return __builtin_popcount(x); }

uint inline bswap(uint x) { return __builtin_bswap32(x); }

int inline clz_vl(uvlong x) { return x ? __builtin_clzll(x) : 64; }

int inline ctz_vl(uvlong x) { return x ? __builtin_ctzll(x) : 64; }

int inline popcnt_vl(uvlong x) { return __builtin_popcountll(x); }

uvlong inline bswap_vl(uvlong x) { return __builtin_bswap64(x); }

uint inline rol(uint x, int k) { return (x << k) | (x >> (32 - k)); }

uint inline ror(uint x, int k) { return (x >> k) | (x << (32 - k)); }

uvlong inline rol_vl(uvlong x, int k) { return (x << k) | (x >> (64 - k)); }

uvlong inline ror_vl(uvlong x, int k) { return (x >> k) | (x << (64 - k)); }

bool inline chkadd(uint a, uint b, uint *out) { return __builtin_add_overflow(a, b, out); }

bool inline chksub(uint a, uint b, uint *out) { return __builtin_sub_overflow(a, b, out); }

bool inline chkmul(uint a, uint b, uint *out) { return __builtin_mul_overflow(a, b, out); }

bool inline chkadd_vl(uvlong a, uvlong b, uvlong *out) { return __builtin_add_overflow(a, b, out); }

bool inline chksub_vl(uvlong a, uvlong b, uvlong *out) { return __builtin_sub_overflow(a, b, out); }

bool inline chkmul_vl(uvlong a, uvlong b, uvlong *out) { return __builtin_mul_overflow(a, b, out); }

#ifdef __SIZEOF_INT128__
  #define HAS_WMUL_VL

urlong inline wmul_vl(uvlong a, uvlong b) {
	unsigned __int128 p = ( unsigned __int128 ) a * b;
	return ( urlong ) {( uvlong ) p, ( uvlong ) (p >> 64)};
}
#endif
