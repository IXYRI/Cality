#pragma once
#include "datom.h"
#include <intrin.h>

// MSVC specific implementations using compiler intrinsics

int inline clz(uint x) {
	unsigned long r;
	return _BitScanReverse(&r, x) ? 31 - r : 32;
}

int inline ctz(uint x) {
	unsigned long r;
	return _BitScanForward(&r, x) ? r : 32;
}

int inline popcnt(uint x) { return __popcnt(x); }

uint inline bswap(uint x) { return _byteswap_ulong(x); }

#ifdef _WIN64
int inline clz_vl(uvlong x) {
	unsigned long r;
	return _BitScanReverse64(&r, x) ? 63 - r : 64;
}

int inline ctz_vl(uvlong x) {
	unsigned long r;
	return _BitScanForward64(&r, x) ? r : 64;
}

int inline popcnt_vl(uvlong x) { return ( int ) __popcnt64(x); }
#endif

uvlong inline bswap_vl(uvlong x) { return _byteswap_uint64(x); }

uint inline rol(uint x, int k) { return _rotl(x, k); }

uint inline ror(uint x, int k) { return _rotr(x, k); }

uvlong inline rol_vl(uvlong x, int k) { return _rotl64(x, k); }

uvlong inline ror_vl(uvlong x, int k) { return _rotr64(x, k); }

#ifdef _WIN64
  #define HAS_WMUL_VL

urlong inline wmul_vl(uvlong a, uvlong b) {
	uvlong h;
	uvlong l = _umul128(a, b, &h);
	return ( urlong ) {l, h};
}
#endif
