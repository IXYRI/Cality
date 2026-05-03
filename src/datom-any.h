#pragma once
#include "datom.h"

// Basic conversions that don't need intrinsics
urlong inline r2u(rlong x) { return ( urlong ) {x.l, x.h}; }

rlong inline u2r(urlong x) { return ( rlong ) {x.l, x.h}; }

utlong inline t2u(tlong x) { return ( utlong ) {x.b, x.d, x.u, x.t}; }

tlong inline u2t(utlong x) { return ( tlong ) {x.b, x.d, x.u, x.t}; }

rlong inline v2r(vlong x) { return ( rlong ) {x, x < 0 ? -1ll : 0ll}; }

tlong inline v2t(vlong x) { return ( tlong ) {x, x < 0 ? -1ll : 0ll, x < 0 ? -1ll : 0ll, x < 0 ? -1ll : 0ll}; }

urlong inline uv2r(uvlong x) { return ( urlong ) {x, 0ull}; }

utlong inline uv2t(uvlong x) { return ( utlong ) {x, 0ull, 0ull, 0ull}; }

// Basic powers of 2 and alignment
bool inline pow2s(uint x) { return x && !(x & (x - 1)); }

bool inline pow2s_vl(uvlong x) { return x && !(x & (x - 1)); }

uint inline alignup(uint x, uint a) { return (x + a - 1) & ~(a - 1); }

uint inline aligndown(uint x, uint a) { return x & ~(a - 1); }

uvlong inline alignup_vl(uvlong x, uvlong a) { return (x + a - 1) & ~(a - 1); }

uvlong inline aligndown_vl(uvlong x, uvlong a) { return x & ~(a - 1); }

#ifndef HAS_WMUL_VL
urlong inline wmul_vl(uvlong a, uvlong b) {
	uvlong al = ( uint ) a, ah = a >> 32;
	uvlong bl = ( uint ) b, bh = b >> 32;
	uvlong l = al * bl, t = ah * bl + (l >> 32);
	uvlong tl = ( uint ) t, th = t >> 32;
	t = al * bh + tl;
	return ( urlong ) {(t << 32) | ( uint ) l, ah * bh + th + (t >> 32)};
}
#endif
