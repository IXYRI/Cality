#pragma once
#include "datom.h"
#include <intrin.h>

// MSVC specific implementations using compiler intrinsics

static inline int clz(uint x) { 
	unsigned long r;
	return _BitScanReverse(&r, x) ? 31 - r : 32; 
}
static inline int ctz(uint x) { 
	unsigned long r;
	return _BitScanForward(&r, x) ? r : 32; 
}
static inline int popcnt(uint x) { return __popcnt(x); }
static inline uint bswap(uint x) { return _byteswap_ulong(x); }

#ifdef _WIN64
static inline int clz_vl(uvlong x) { 
	unsigned long r;
	return _BitScanReverse64(&r, x) ? 63 - r : 64; 
}
static inline int ctz_vl(uvlong x) { 
	unsigned long r;
	return _BitScanForward64(&r, x) ? r : 64; 
}
static inline int popcnt_vl(uvlong x) { return __popcnt64(x); }
#endif

static inline uvlong bswap_vl(uvlong x) { return _byteswap_uint64(x); }

static inline uint rol(uint x, int k) { return _rotl(x, k); }
static inline uint ror(uint x, int k) { return _rotr(x, k); }
static inline uvlong rol_vl(uvlong x, int k) { return _rotl64(x, k); }
static inline uvlong ror_vl(uvlong x, int k) { return _rotr64(x, k); }
