#pragma once
#include "datom.h"

// Clang & GCC specific implementations using __builtin

static inline int clz(uint x) { return x ? __builtin_clz(x) : 32; }
static inline int ctz(uint x) { return x ? __builtin_ctz(x) : 32; }
static inline int popcnt(uint x) { return __builtin_popcount(x); }
static inline uint bswap(uint x) { return __builtin_bswap32(x); }

static inline int clz_vl(uvlong x) { return x ? __builtin_clzll(x) : 64; }
static inline int ctz_vl(uvlong x) { return x ? __builtin_ctzll(x) : 64; }
static inline int popcnt_vl(uvlong x) { return __builtin_popcountll(x); }
static inline uvlong bswap_vl(uvlong x) { return __builtin_bswap64(x); }

static inline uint rol(uint x, int k) { return (x << k) | (x >> (32 - k)); }
static inline uint ror(uint x, int k) { return (x >> k) | (x << (32 - k)); }
static inline uvlong rol_vl(uvlong x, int k) { return (x << k) | (x >> (64 - k)); }
static inline uvlong ror_vl(uvlong x, int k) { return (x >> k) | (x << (64 - k)); }

static inline bool chkadd(uint a, uint b, uint *out) { return __builtin_add_overflow(a, b, out); }
static inline bool chksub(uint a, uint b, uint *out) { return __builtin_sub_overflow(a, b, out); }
static inline bool chkmul(uint a, uint b, uint *out) { return __builtin_mul_overflow(a, b, out); }

static inline bool chkadd_vl(uvlong a, uvlong b, uvlong *out) { return __builtin_add_overflow(a, b, out); }
static inline bool chksub_vl(uvlong a, uvlong b, uvlong *out) { return __builtin_sub_overflow(a, b, out); }
static inline bool chkmul_vl(uvlong a, uvlong b, uvlong *out) { return __builtin_mul_overflow(a, b, out); }
