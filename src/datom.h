#pragma once
#if LONG_MAX != INT_MAX
  #define vlong long
#else
  #define vlong long long
#endif
#define uchar  unsigned char
#define ushort unsigned short
#define uint   unsigned int
#define ulong  unsigned long
#define uvlong unsigned vlong
#define rune   uint
#define null   ( void * ) 0

/* Bool Compatibility */
#if !true
  #define bool  _Bool
  #define true  1
  #define false 0
#endif

/* As MSVC doesn't support 128-bit integers, structs are used instead. */
typedef struct {
	uvlong l;
	vlong  h;
} rlong;

typedef struct {
	uvlong l;
	uvlong h;
} urlong;

typedef struct {
	uvlong b;
	uvlong d;
	uvlong u;
	vlong  t;
} tlong;

typedef struct {
	uvlong b;
	uvlong d;
	uvlong u;
	uvlong t;
} utlong;

static urlong inline r2u(rlong x) { return ( urlong ) {x.l, x.h}; }

static rlong inline u2r(urlong x) { return ( rlong ) {x.l, x.h}; }

static utlong inline t2u(tlong x) { return ( utlong ) {x.b, x.d, x.u, x.t}; }

static tlong inline u2t(utlong x) { return ( tlong ) {x.b, x.d, x.u, x.t}; }

#define sigpad(x) ( vlong ) (x < 0 ? -1 : 0)

static rlong inline v2r(vlong x) { return ( rlong ) {x, sigpad(x)}; }

static tlong inline v2t(vlong x) { return ( tlong ) {x, sigpad(x), sigpad(x), sigpad(x)}; }

static urlong inline uv2r(uvlong x) { return ( urlong ) {x, 0}; }

static utlong inline uv2t(uvlong x) { return ( utlong ) {x, 0, 0, 0}; }

static tlong inline r2t(rlong x) { return ( tlong ) {x.l, x.h, sigpad(x.h), sigpad(x.h)}; }

static vlong inline r2v(rlong x) { return x.l; }

static utlong inline ur2t(urlong x) { return ( utlong ) {x.l, x.h, 0, 0}; }

static uvlong inline ur2v(urlong x) { return x.l; }

static rlong inline t2r(tlong x) { return ( rlong ) {x.b, x.d, sigpad(x.t), sigpad(x.t)}; }

static vlong inline t2v(tlong x) { return x.b; }

static urlong inline ut2r(utlong x) { return ( urlong ) {x.b, x.d}; }

static uvlong inline ut2v(utlong x) { return x.b; }

static urlong inline and_rl(rlong a, rlong b) { return ( urlong ) {a.l & b.l, a.h & b.h}; }

static urlong inline or_rl(rlong a, rlong b) { return ( urlong ) {a.l | b.l, a.h | b.h}; }

static urlong inline xor_rl(rlong a, rlong b) { return ( urlong ) {a.l ^ b.l, a.h ^ b.h}; }

static urlong inline not_rl(rlong x) { return ( urlong ) {~x.l, ~x.h}; }

static utlong inline and_tl(tlong a, tlong b) { return ( utlong ) {a.b & b.b, a.d & b.d, a.u & b.u, a.t & b.t}; }

static utlong inline or_tl(tlong a, tlong b) { return ( utlong ) {a.b | b.b, a.d | b.d, a.u | b.u, a.t | b.t}; }

static utlong inline xor_tl(tlong a, tlong b) { return ( utlong ) {a.b ^ b.b, a.d ^ b.d, a.u ^ b.u, a.t ^ b.t}; }

static utlong inline not_tl(tlong x) { return ( utlong ) {~x.b, ~x.d, ~x.u, ~x.t}; }

typedef struct quorem {
	uint q, r;
} quorem;

typedef struct quorem_rl {
	urlong q, r;
} quorem_rl;

typedef struct quorem_tl {
	utlong q, r;
} quorem_tl;

typedef struct biut {
	utlong l, h;
} biut;

// #if defined(__clang__) || defined(__GNUC__)
//   #include "datom-cc.h"
// #elif defined(_MSC_VER)
//   #include "datom-msvc.h"
// #else /* datom-any.h */
/* Bit Manipulation */
static int inline clz(uint x) {
	int n = 0;
	if (!x) return 32;
	if (!(x & 0xffff0000)) n += 16, x <<= 16;
	if (!(x & 0xff000000)) n += 8, x <<= 8;
	if (!(x & 0xf0000000)) n += 4, x <<= 4;
	if (!(x & 0xc0000000)) n += 2, x <<= 2;
	if (!(x & 0x80000000)) n += 1;
	return n;
}

static int inline ctz(uint x) {
	int n = 0;
	if (!x) return 32;
	if (!(x & 0x0000ffff)) n += 16, x >>= 16;
	if (!(x & 0x000000ff)) n += 8, x >>= 8;
	if (!(x & 0x0000000f)) n += 4, x >>= 4;
	if (!(x & 0x00000003)) n += 2, x >>= 2;
	if (!(x & 0x00000001)) n += 1;
	return n;
}

static int inline popcnt(uint x) {
	x = x - ((x >> 1) & 0x55555555);
	x = (x & 0x33333333) + ((x >> 2) & 0x33333333);
	return (((x + (x >> 4)) & 0x0f0f0f0f) * 0x01010101) >> 24;
}

static uint inline bswap(uint x) { return (x >> 24) | ((x >> 8) & 0x0000ff00) | ((x << 8) & 0x00ff0000) | (x << 24); }

static uint inline bitrev(uint x) {
	x = ((x & 0x55555555) << 1) | ((x >> 1) & 0x55555555);
	x = ((x & 0x33333333) << 2) | ((x >> 2) & 0x33333333);
	x = ((x & 0x0f0f0f0f) << 4) | ((x >> 4) & 0x0f0f0f0f);
	return bswap(x);
}

static int inline clz_vl(uvlong x) {
	int n = 0;
	if (!x) return 64;
	if (!(x & 0xffffffff00000000)) n += 32, x <<= 32;
	if (!(x & 0xffff000000000000)) n += 16, x <<= 16;
	if (!(x & 0xff00000000000000)) n += 8, x <<= 8;
	if (!(x & 0xf000000000000000)) n += 4, x <<= 4;
	if (!(x & 0xc000000000000000)) n += 2, x <<= 2;
	if (!(x & 0x8000000000000000)) n += 1;
	return n;
}

static int inline ctz_vl(uvlong x) {
	int n = 0;
	if (!x) return 64;
	if (!(x & 0x00000000ffffffff)) n += 32, x >>= 32;
	if (!(x & 0x000000000000ffff)) n += 16, x >>= 16;
	if (!(x & 0x00000000000000ff)) n += 8, x >>= 8;
	if (!(x & 0x000000000000000f)) n += 4, x >>= 4;
	if (!(x & 0x0000000000000003)) n += 2, x >>= 2;
	if (!(x & 0x0000000000000001)) n += 1;
	return n;
}

static int inline popcnt_vl(uvlong x) {
	x = x - ((x >> 1) & 0x5555555555555555);
	x = (x & 0x3333333333333333) + ((x >> 2) & 0x3333333333333333);
	return (((x + (x >> 4)) & 0x0f0f0f0f0f0f0f0f) * 0x0101010101010101) >> 56;
}

static uvlong inline bswap_vl(uvlong x) {
	return (x >> 56)
	     | ((x >> 40) & 0x000000000000ff00)
	     | ((x >> 24) & 0x0000000000ff0000)
	     | ((x >> 8) & 0x00000000ff000000)
	     | ((x << 8) & 0x000000ff00000000)
	     | ((x << 24) & 0x0000ff0000000000)
	     | ((x << 40) & 0x00ff000000000000)
	     | (x << 56);
}

static uvlong inline bitrev_vl(uvlong x) {
	x = ((x & 0x5555555555555555) << 1) | ((x >> 1) & 0x5555555555555555);
	x = ((x & 0x3333333333333333) << 2) | ((x >> 2) & 0x3333333333333333);
	x = ((x & 0x0f0f0f0f0f0f0f0f) << 4) | ((x >> 4) & 0x0f0f0f0f0f0f0f0f);
	return bswap_vl(x);
}

static int inline clz_rl(urlong x) {
	int n = 0;
	if (!(x.l | x.h)) return 128;
	return x.h ? clz_vl(x.h) : 64 + clz_vl(x.l);
}

static int inline ctz_rl(urlong x) {
	int n = 0;
	if (!(x.l | x.h)) return 128;
	return x.l ? ctz_vl(x.l) : 64 + ctz_vl(x.h);
}

static int inline popcnt_rl(urlong x) { return popcnt_vl(x.l) + popcnt_vl(x.h); }

static urlong inline bswap_rl(urlong x) { return ( urlong ) {bswap_vl(x.h), bswap_vl(x.l)}; }

static urlong inline bitrev_rl(urlong x) { return ( urlong ) {bitrev_vl(x.h), bitrev_vl(x.l)}; }

static int inline clz_tl(utlong x) {
	if (!(x.b | x.d | x.u | x.t)) return 256;
	if (x.t) return clz_vl(x.t);
	if (x.u) return 64 + clz_vl(x.u);
	if (x.d) return 128 + clz_vl(x.d);
	return 192 + clz_vl(x.b);
}

static int inline ctz_tl(utlong x) {
	if (!(x.b | x.d | x.u | x.t)) return 256;
	if (x.b) return ctz_vl(x.b);
	if (x.d) return 64 + ctz_vl(x.d);
	if (x.u) return 128 + ctz_vl(x.u);
	return 192 + ctz_vl(x.t);
}

static int inline popcnt_tl(utlong x) { return popcnt_vl(x.b) + popcnt_vl(x.d) + popcnt_vl(x.u) + popcnt_vl(x.t); }

static utlong inline bswap_tl(utlong x) {
	return ( utlong ) {bswap_vl(x.t), bswap_vl(x.u), bswap_vl(x.d), bswap_vl(x.b)};
}

static utlong inline bitrev_tl(utlong x) {
	return ( utlong ) {bitrev_vl(x.t), bitrev_vl(x.u), bitrev_vl(x.d), bitrev_vl(x.b)};
}

/* Rotation and Shift */
static urlong inline shl_rl(urlong x, int k) {
	return k %= 128, k < 64 ? ( urlong ) {x.l << k, (x.h << k) | (x.l >> (64 - k))} : ( urlong ) {0, x.l << (k - 64)};
}

static urlong inline shr_rl(urlong x, int k) {
	return k %= 128, k < 64 ? ( urlong ) {(x.l >> k) | (x.h << (64 - k)), x.h >> k} : ( urlong ) {x.h >> (k - 64), 0};
}

static rlong inline sar_rl(rlong x, int k) {
	return k %= 128, k < 64 ? ( rlong ) {(x.l >> k) | (x.h << (64 - k)), x.h >> k} : ( rlong ) {x.h >> (k - 64), -1};
}

static utlong inline shl_tl(utlong x, int k) {
	return k %= 256, k < 64    ? ( utlong ) {x.b << k, (x.d << k) | (x.b >> (64 - k)), (x.u << k) | (x.d >> (64 - k)),
	                                         (x.t << k) | (x.u >> (64 - k))}
	                 : k < 128 ? ( utlong ) {0, x.b << (k - 64), (x.d << (k - 64)) | (x.b >> (128 - k)),
	                                         (x.u << (k - 64)) | (x.d >> (128 - k))}
	                 : k < 192 ? ( utlong ) {0, 0, x.b << (k - 128), (x.d << (k - 128)) | (x.b >> (192 - k))}
	                           : ( utlong ) {0, 0, 0, x.b << (k - 192)};
}

static utlong inline shr_tl(utlong x, int k) {
	return k %= 256, k < 64    ? ( utlong ) {(x.b >> k) | (x.d << (64 - k)), (x.d >> k) | (x.u << (64 - k)),
	                                         (x.u >> k) | (x.t << (64 - k)), x.t >> k}
	                 : k < 128 ? ( utlong ) {(x.d >> (k - 64)) | (x.u << (128 - k)),
	                                         (x.u >> (k - 64)) | (x.t << (128 - k)), x.t >> (k - 64), 0}
	                 : k < 192 ? ( utlong ) {(x.u >> (k - 128)) | (x.t << (192 - k)), x.t >> (k - 128), 0, 0}
	                           : ( utlong ) {x.t >> (k - 192), 0, 0, 0};
}

static tlong inline sar_tl(tlong x, int k) {
	return k %= 256, k < 64    ? ( tlong ) {(x.b >> k) | (x.d << (64 - k)), (x.d >> k) | (x.u << (64 - k)),
	                                        (x.u >> k) | (x.t << (64 - k)), x.t >> k}
	                 : k < 128 ? ( tlong ) {(x.d >> (k - 64)) | (x.u << (128 - k)),
	                                        (x.u >> (k - 64)) | (x.t << (128 - k)), x.t >> (k - 64), sigpad(x.t)}
	                 : k < 192
	                   ? ( tlong ) {(x.u >> (k - 128)) | (x.t << (192 - k)), x.t >> (k - 128), sigpad(x.t), sigpad(x.t)}
	                   : ( tlong ) {x.t >> (k - 192), sigpad(x.t), sigpad(x.t), sigpad(x.t)};
}

static uint inline rol(uint x, int k) { return k %= 32, (x << k) | (x >> (32 - k)); }

static uint inline ror(uint x, int k) { return k %= 32, (x >> k) | (x << (32 - k)); }

static uvlong inline rol_vl(uvlong x, int k) { return k %= 64, (x << k) | (x >> (64 - k)); }

static uvlong inline ror_vl(uvlong x, int k) { return k %= 64, (x >> k) | (x << (64 - k)); }

static urlong inline rol_rl(urlong x, int k) { return k %= 128, or_rl(shl_rl(x, k), shr_rl(x, 128 - k)); }

static urlong inline ror_rl(urlong x, int k) { return k %= 128, or_rl(shr_rl(x, k), shl_rl(x, 128 - k)); }

static utlong inline rol_tl(utlong x, int k) { return k %= 256, or_tl(shl_tl(x, k), shr_tl(x, 256 - k)); }

static utlong inline ror_tl(utlong x, int k) { return k %= 256, or_tl(shr_tl(x, k), shl_tl(x, 256 - k)); }

/* Arithmetic */
static uvlong inline clmul(uint a, uint b) {
	uvlong r = 0;
	for (int i = 0; i < 32; i++)
		if (b & (1u << i)) r ^= ( uvlong ) a << i;
	return r;
}

static quorem inline divmod(uint a, uint b) { return ( quorem ) {a / b, a % b}; }

static urlong inline add_rl(urlong a, urlong b) {
	uvlong l = a.l + b.l;
	return ( urlong ) {l, a.h + b.h + (l < a.l)};
}

static urlong inline sub_rl(urlong a, urlong b) {
	uvlong l = a.l - b.l;
	return ( urlong ) {l, a.h - b.h - (l > a.l)};
}

static urlong inline clmul_vl(uvlong a, uvlong b) {
	uvlong lo = b & 1 ? a : 0, hi = 0;
	for (int i = 1; i < 64; i++)
		if (b & (( uvlong ) 1 << i)) lo ^= a << i, hi ^= a >> (64 - i);
	return ( urlong ) {lo, hi};
}

static urlong inline wmul_vl(uvlong a, uvlong b) {
	return ( urlong ) {
	  a * b, (a >> 32) * (b >> 32) + ((a >> 32) * (b & 0xffffffff) >> 32) + ((b >> 32) * (a & 0xffffffff) >> 32)};
}

static quorem_vl inline divmod_vl(uvlong a, uvlong b) { return ( quorem_vl ) {a / b, a % b}; }

static urlong inline mul_rl(urlong a, urlong b) { return add_rl(a.l * b.l, a.h * b.l + b.h * a.l << 32); }

static quorem_rl inline divmod_rl(urlong a, urlong b) {
	assert(b.l | b.h);
	int shift = clz_rl(b) - clz_rl(a);
	if (shift < 0) return ( quorem_rl ) {{}, a};
	urlong q = {}, r = a, d = shl_rl(b, shift);
	for (int i = 0; i <= shift; d = shr_rl(d, 1), i++)
		if (r.h > d.h || (r.h == d.h && r.l >= d.l))
			r = sub_rl(r, d),
			shift - i < 64 ? q.l |= ( uvlong ) 1 << (shift - i) : (q.h |= ( uvlong ) 1 << (shift - i - 64));
	return ( quorem_rl ) {q, r};
}

static urlong inline div_rl(urlong a, urlong b) { return divmod_rl(a, b).q; }

static urlong inline mod_rl(urlong a, urlong b) { return divmod_rl(a, b).r; }

static uint inline adc(uint a, uint b, bool ci, bool *co) {
	uint r = a + b + ci;
	if (co) *co = r < a || ci && r == a;
	return r;
}

static uint inline sbb(uint a, uint b, bool bi, bool *bo) {
	uint r = a - b - bi;
	if (bo) *bo = a < b || bi && a == b;
	return r;
}

static uvlong inline adc_vl(uvlong a, uvlong b, bool ci, bool *co) {
	uvlong r = a + b + ci;
	if (co) *co = r < a || ci && r == a;
	return r;
}

static uvlong inline sbb_vl(uvlong a, uvlong b, bool bi, bool *bo) {
	uvlong r = a - b - bi;
	if (bo) *bo = a < b || bi && a == b;
	return r;
}

static urlong inline adc_rl(urlong a, urlong b, bool ci, bool *co) {
	bool c;
	return (urlong) { adc_vl(a.l, b.l, ci, &c), adc_vl(a.h, b.h, c, co) }
}

static urlong inline sbb_rl(urlong a, urlong b, bool bi, bool *bo) {
	bool c;
	return (urlong) { sbb_vl(a.l, b.l, bi, &c), sbb_vl(a.h, b.h, c, bo) }
}

static utlong inline adc_tl(utlong a, utlong b, bool ci, bool *co) {
	bool c;
	return ( utlong ) {
	  adc_vl(a.b, b.b, ci, &c),
	  adc_vl(a.d, b.d, c, &c),
	  adc_vl(a.u, b.u, c, &c),
	  adc_vl(a.t, b.t, c, co),
	};
}

static utlong inline sbb_tl(utlong a, utlong b, bool bi, bool *bo) {
	bool c;
	return ( utlong ) {
	  sbb_vl(a.b, b.b, bi, &c),
	  sbb_vl(a.d, b.d, c, &c),
	  sbb_vl(a.u, b.u, c, &c),
	  sbb_vl(a.t, b.t, c, bo),
	};
}

static utlong inline add_tl(utlong a, utlong b) {
	bool c;
	return ( utlong ) {
	  adc_vl(a.b, b.b, false, &c),
	  adc_vl(a.d, b.d, c, &c),
	  adc_vl(a.u, b.u, c, &c),
	  a.t + b.t + c,
	};
}

static utlong inline sub_tl(utlong a, utlong b) {
	bool c;
	return ( utlong ) {
	  sbb_vl(a.b, b.b, false, &c),
	  sbb_vl(a.d, b.d, c, &c),
	  sbb_vl(a.u, b.u, c, &c),
	  a.t - b.t - c,
	};
}

static utlong inline clmul_rl(urlong a, urlong b) {
	urlong l = clmul_vl(a.l, b.l), h = clmul_vl(a.h, b.h), m = xor_rl(clmul_vl(a.l, b.h), clmul_vl(a.h, b.l));
	return ( utlong ) {l.l, l.h ^ m.l, h.l ^ m.h, h.h};
}

static utlong inline wmul_rl(urlong a, urlong b) {
	// urlong ll = wmul_vl(a.l, b.l), lh = wmul_vl(a.l, b.h);
	// urlong hl = wmul_vl(a.h, b.l), hh = wmul_vl(a.h, b.h);
	// utlong res = {ll.l, ll.h, 0, 0};
	// res        = add_tl(res, ( utlong ) {0, lh.l, lh.h, 0});
	// res        = add_tl(res, ( utlong ) {0, hl.l, hl.h, 0});
	// res        = add_tl(res, ( utlong ) {0, 0, hh.l, hh.h});
	// return res;
	urlong bd = mul_rl(a, b), ut = wmul_vl(a.h, b.h);
	return (utlong) { bd.l, bd.h, (a.h * b.l >> 32) + (b.h * a.l >> 32) + ut.l, ut.h }
}

static utlong inline mul_tl(utlong a, utlong b);
static utlong inline div_tl(utlong a, utlong b);
static utlong inline mod_tl(utlong a, utlong b);
static biut inline clmul_tl(utlong a, utlong b);
static biut inline wmul_tl(utlong a, utlong b);
static quorem_tl inline divmod_tl(utlong a, utlong b);

static rlong inline add_sr(rlong a, rlong b);
static rlong inline sub_sr(rlong a, rlong b);
static rlong inline mul_sr(rlong a, rlong b);
static rlong inline div_sr(rlong a, rlong b);
static rlong inline mod_sr(rlong a, rlong b);
static rlong inline neg_sr(rlong x);
static tlong inline add_st(tlong a, tlong b);
static tlong inline sub_st(tlong a, tlong b);
static tlong inline mul_st(tlong a, tlong b);
static tlong inline div_st(tlong a, tlong b);
static tlong inline mod_st(tlong a, tlong b);
static tlong inline neg_st(tlong x);

/* Power of 2 and Alignment */
static bool inline pow2s(uint x) { return x && !(x & (x - 1)); }

static uint inline nxtp2(uint x) { return x ? 1u << (32 - clz(x - 1)) : 1; }

static uint inline alignup(uint x, uint a) { return (x + a - 1) & ~(a - 1); }

static uint inline aligndown(uint x, uint a) { return x & ~(a - 1); }

static bool inline pow2s_vl(uvlong x) { return x && !(x & (x - 1)); }

static uvlong inline nxtp2_vl(uvlong x) { return x ? ( uvlong ) 1 << (64 - clz_vl(x - 1)) : 1; }

static uvlong inline alignup_vl(uvlong x, uvlong a) { return (x + a - 1) & ~(a - 1); }

static uvlong inline aligndown_vl(uvlong x, uvlong a) { return x & ~(a - 1); }

static bool inline pow2s_rl(urlong x) { return !x.h && pow2s_vl(x.l) || !x.l && pow2s_vl(x.h); }

static urlong inline nxtp2_rl(urlong x) {
	return x.h                   ? ( urlong ) {0, nxtp2_vl(x.h)}
	     : x.l >> 63 && x.l << 1 ? ( urlong ) {0, 1}
	                             : ( urlong ) {nxtp2_vl(x.l), 0};
}

static urlong inline alignup_rl(urlong x, urlong a) {
	urlong one = {1, 0}, a_minus_1 = sub_rl(a, one), x_plus = add_rl(x, a_minus_1), mask = not_rl(a_minus_1);
	return and_rl(x_plus, mask);
}

static urlong inline aligndown_rl(urlong x, urlong a) {
	urlong one = {1, 0}, a_minus_1 = sub_rl(a, one), mask = not_rl(a_minus_1);
	return and_rl(x, mask);
}

static bool inline pow2s_tl(utlong x) {
	return !(x.b | x.d | x.u) && pow2s_vl(x.t)
	    || !(x.b | x.d | x.t) && pow2s_vl(x.u)
	    || !(x.b | x.u | x.t) && pow2s_vl(x.d)
	    || !(x.d | x.u | x.t) && pow2s_vl(x.b);
}

static utlong inline nxtp2_tl(utlong x) {
	if (x.t) return ( utlong ) {0, 0, 0, nxtp2_vl(x.t)};
	if (x.u) return x.u >> 63 && x.u << 1 ? ( utlong ) {0, 0, 0, 1} : ( utlong ) {0, 0, nxtp2_vl(x.u), 0};
	if (x.d) return x.d >> 63 && x.d << 1 ? ( utlong ) {0, 0, 1, 0} : ( utlong ) {0, nxtp2_vl(x.d), 0, 0};
	return x.b >> 63 && x.b << 1 ? ( utlong ) {0, 1, 0, 0} : ( utlong ) {nxtp2_vl(x.b), 0, 0, 0};
}

static utlong inline alignup_tl(utlong x, utlong a) {
	utlong one = {1, 0, 0, 0}, a_minus_1 = sub_tl(a, one), x_plus = add_tl(x, a_minus_1), mask = not_tl(a_minus_1);
	return and_tl(x_plus, mask);
}

static utlong inline aligndown_tl(utlong x, utlong a) {
	utlong one = {1, 0, 0, 0}, a_minus_1 = sub_tl(a, one), mask = not_tl(a_minus_1);
	return and_tl(x, mask);
}

/* Checked Arithmetic */
static bool inline chkadd(uint a, uint b, uint *out);
static bool inline chksub(uint a, uint b, uint *out);
static bool inline chkmul(uint a, uint b, uint *out);
static bool inline chkadd_vl(uvlong a, uvlong b, uvlong *out);
static bool inline chksub_vl(uvlong a, uvlong b, uvlong *out);
static bool inline chkmul_vl(uvlong a, uvlong b, uvlong *out);
static bool inline chkadd_rl(urlong a, urlong b, urlong *out);
static bool inline chksub_rl(urlong a, urlong b, urlong *out);
static bool inline chkmul_rl(urlong a, urlong b, urlong *out);
static bool inline chkadd_tl(utlong a, utlong b, utlong *out);
static bool inline chksub_tl(utlong a, utlong b, utlong *out);
static bool inline chkmul_tl(utlong a, utlong b, utlong *out);

static bool inline chkadd_s(int a, int b, int *out);
static bool inline chksub_s(int a, int b, int *out);
static bool inline chkmul_s(int a, int b, int *out);
static bool inline chkdiv_s(int a, int b, int *out);
static bool inline chkadd_sv(vlong a, vlong b, vlong *out);
static bool inline chksub_sv(vlong a, vlong b, vlong *out);
static bool inline chkmul_sv(vlong a, vlong b, vlong *out);
static bool inline chkdiv_sv(vlong a, vlong b, vlong *out);
static bool inline chkadd_sr(rlong a, rlong b, rlong *out);
static bool inline chksub_sr(rlong a, rlong b, rlong *out);
static bool inline chkmul_sr(rlong a, rlong b, rlong *out);
static bool inline chkdiv_sr(rlong a, rlong b, rlong *out);
static bool inline chkadd_st(tlong a, tlong b, tlong *out);
static bool inline chksub_st(tlong a, tlong b, tlong *out);
static bool inline chkmul_st(tlong a, tlong b, tlong *out);
static bool inline chkdiv_st(tlong a, tlong b, tlong *out);

/* Comparison */
static int inline cmp_rl(rlong a, rlong b);
static int inline cmp_tl(tlong a, tlong b);
static int inline cmp_sr(rlong a, rlong b);
static int inline cmp_st(tlong a, tlong b);
// #endif
