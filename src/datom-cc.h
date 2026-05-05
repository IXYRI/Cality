#pragma once
#include "datom.h"

// Clang & GCC specific implementations using __builtin

static int inline clz(uint x) { return x ? __builtin_clz(x) : 32; }

static int inline ctz(uint x) { return x ? __builtin_ctz(x) : 32; }

static int inline popcnt(uint x) { return __builtin_popcount(x); }

static uint inline bswap(uint x) { return __builtin_bswap32(x); }

static int inline clz_vl(uvlong x) { return x ? __builtin_clzll(x) : 64; }

static int inline ctz_vl(uvlong x) { return x ? __builtin_ctzll(x) : 64; }

static int inline popcnt_vl(uvlong x) { return __builtin_popcountll(x); }

static uvlong inline bswap_vl(uvlong x) { return __builtin_bswap64(x); }

static uint inline rol(uint x, int k) { return (x << k) | (x >> (32 - k)); }

static uint inline ror(uint x, int k) { return (x >> k) | (x << (32 - k)); }

static uvlong inline rol_vl(uvlong x, int k) { return (x << k) | (x >> (64 - k)); }

static uvlong inline ror_vl(uvlong x, int k) { return (x >> k) | (x << (64 - k)); }

static bool inline chkadd(uint a, uint b, uint *out) { return __builtin_add_overflow(a, b, out); }

static bool inline chksub(uint a, uint b, uint *out) { return __builtin_sub_overflow(a, b, out); }

static bool inline chkmul(uint a, uint b, uint *out) { return __builtin_mul_overflow(a, b, out); }

static bool inline chkadd_vl(uvlong a, uvlong b, uvlong *out) { return __builtin_add_overflow(a, b, out); }

static bool inline chksub_vl(uvlong a, uvlong b, uvlong *out) { return __builtin_sub_overflow(a, b, out); }

static bool inline chkmul_vl(uvlong a, uvlong b, uvlong *out) { return __builtin_mul_overflow(a, b, out); }

#ifdef __SIZEOF_INT128__
  #define HAS_WMUL_VL

static urlong inline wmul_vl(uvlong a, uvlong b) {
	unsigned __int128 p = ( unsigned __int128 ) a * b;
	return ( urlong ) {( uvlong ) p, ( uvlong ) (p >> 64)};
}
#endif
