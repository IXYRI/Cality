#pragma once
#include "datom.h"

// Basic conversions that don't need intrinsics
static inline urlong r2u(rlong x) { return (urlong){x.l, x.h}; }
static inline rlong  u2r(urlong x) { return (rlong){x.l, x.h}; }
static inline utlong t2u(tlong x) { return (utlong){x.b, x.d, x.u, x.t}; }
static inline tlong  u2t(utlong x) { return (tlong){x.b, x.d, x.u, x.t}; }

static inline rlong  v2r(vlong x) { return (rlong){x, x < 0 ? -1LL : 0LL}; }
static inline tlong  v2t(vlong x) { return (tlong){x, x < 0 ? -1LL : 0LL, x < 0 ? -1LL : 0LL, x < 0 ? -1LL : 0LL}; }
static inline urlong uv2r(uvlong x) { return (urlong){x, 0ULL}; }
static inline utlong uv2t(uvlong x) { return (utlong){x, 0ULL, 0ULL, 0ULL}; }

// Basic powers of 2 and alignment
static inline bool pow2s(uint x) { return x && !(x & (x - 1)); }
static inline bool pow2s_vl(uvlong x) { return x && !(x & (x - 1)); }

static inline uint alignup(uint x, uint a) { return (x + a - 1) & ~(a - 1); }
static inline uint aligndown(uint x, uint a) { return x & ~(a - 1); }
static inline uvlong alignup_vl(uvlong x, uvlong a) { return (x + a - 1) & ~(a - 1); }
static inline uvlong aligndown_vl(uvlong x, uvlong a) { return x & ~(a - 1); }
