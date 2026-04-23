#pragma once
#if sizeof(long) == 8
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

/* Type Conversion */
urlong r2u(rlong x);
rlong  u2r(urlong x);
utlong t2u(tlong x);
tlong  u2t(utlong x);
rlong  v2r(vlong x);
tlong  v2t(vlong x);
urlong uv2r(uvlong x);
utlong uv2t(uvlong x);
tlong  r2t(rlong x);
vlong  r2v(rlong x);
utlong ur2t(urlong x);
uvlong ur2v(urlong x);
rlong  t2r(tlong x);
vlong  t2v(tlong x);
urlong ut2r(utlong x);
uvlong ut2v(utlong x);

/* Bit Manipulation */
int    clz(uint x);
int    ctz(uint x);
int    popcnt(uint x);
uint   bswap(uint x);
uint   bitrev(uint x);
int    clz_vl(uvlong x);
int    ctz_vl(uvlong x);
int    popcnt_vl(uvlong x);
uvlong bswap_vl(uvlong x);
uvlong bitrev_vl(uvlong x);
int    clz_rl(urlong x);
int    ctz_rl(urlong x);
int    popcnt_rl(urlong x);
urlong bswap_rl(urlong x);
urlong bitrev_rl(urlong x);
int    clz_tl(utlong x);
int    ctz_tl(utlong x);
int    popcnt_tl(utlong x);
utlong bswap_tl(utlong x);
utlong bitrev_tl(utlong x);

/* Rotation and Shift */
uint   rol(uint x, int k);
uint   ror(uint x, int k);
uvlong rol_vl(uvlong x, int k);
uvlong ror_vl(uvlong x, int k);
urlong rol_rl(urlong x, int k);
urlong ror_rl(urlong x, int k);
utlong rol_tl(utlong x, int k);
utlong ror_tl(utlong x, int k);

urlong shl_rl(urlong x, int k);
urlong shr_rl(urlong x, int k);
urlong sar_rl(urlong x, int k);
utlong shl_tl(utlong x, int k);
utlong shr_tl(utlong x, int k);
utlong sar_tl(utlong x, int k);

/* Power of 2 and Alignment */
bool   pow2s(uint x);
uint   nxtp2(uint x);
uint   alignup(uint x, uint a);
uint   aligndown(uint x, uint a);
bool   pow2s_vl(uvlong x);
uvlong nxtp2_vl(uvlong x);
uvlong alignup_vl(uvlong x, uvlong a);
uvlong aligndown_vl(uvlong x, uvlong a);
bool   pow2s_rl(urlong x);
urlong nxtp2_rl(urlong x);
urlong alignup_rl(urlong x, urlong a);
urlong aligndown_rl(urlong x, urlong a);
bool   pow2s_tl(utlong x);
utlong nxtp2_tl(utlong x);
utlong alignup_tl(utlong x, utlong a);
utlong aligndown_tl(utlong x, utlong a);

/* Arithmetic */
urlong add_rl(urlong a, urlong b);
urlong sub_rl(urlong a, urlong b);
urlong mul_rl(urlong a, urlong b);
urlong div_rl(urlong a, urlong b);
urlong mod_rl(urlong a, urlong b);
utlong add_tl(utlong a, utlong b);
utlong sub_tl(utlong a, utlong b);
utlong mul_tl(utlong a, utlong b);
utlong div_tl(utlong a, utlong b);
utlong mod_tl(utlong a, utlong b);

rlong  add_sr(rlong a, rlong b);
rlong  sub_sr(rlong a, rlong b);
rlong  mul_sr(rlong a, rlong b);
rlong  div_sr(rlong a, rlong b);
rlong  mod_sr(rlong a, rlong b);
rlong  neg_sr(rlong x);
tlong  add_st(tlong a, tlong b);
tlong  sub_st(tlong a, tlong b);
tlong  mul_st(tlong a, tlong b);
tlong  div_st(tlong a, tlong b);
tlong  mod_st(tlong a, tlong b);
tlong  neg_st(tlong x);

/* Checked Arithmetic */
bool   chkadd(uint a, uint b, uint *out);
bool   chksub(uint a, uint b, uint *out);
bool   chkmul(uint a, uint b, uint *out);
bool   chkadd_vl(uvlong a, uvlong b, uvlong *out);
bool   chksub_vl(uvlong a, uvlong b, uvlong *out);
bool   chkmul_vl(uvlong a, uvlong b, uvlong *out);
bool   chkadd_rl(urlong a, urlong b, urlong *out);
bool   chksub_rl(urlong a, urlong b, urlong *out);
bool   chkmul_rl(urlong a, urlong b, urlong *out);
bool   chkadd_tl(utlong a, utlong b, utlong *out);
bool   chksub_tl(utlong a, utlong b, utlong *out);
bool   chkmul_tl(utlong a, utlong b, utlong *out);

bool   chkadd_s(int a, int b, int *out);
bool   chksub_s(int a, int b, int *out);
bool   chkmul_s(int a, int b, int *out);
bool   chkdiv_s(int a, int b, int *out);
bool   chkadd_sv(vlong a, vlong b, vlong *out);
bool   chksub_sv(vlong a, vlong b, vlong *out);
bool   chkmul_sv(vlong a, vlong b, vlong *out);
bool   chkdiv_sv(vlong a, vlong b, vlong *out);
bool   chkadd_sr(rlong a, rlong b, rlong *out);
bool   chksub_sr(rlong a, rlong b, rlong *out);
bool   chkmul_sr(rlong a, rlong b, rlong *out);
bool   chkdiv_sr(rlong a, rlong b, rlong *out);
bool   chkadd_st(tlong a, tlong b, tlong *out);
bool   chksub_st(tlong a, tlong b, tlong *out);
bool   chkmul_st(tlong a, tlong b, tlong *out);
bool   chkdiv_st(tlong a, tlong b, tlong *out);

/* Carry and Borrow */
uint   adc(uint a, uint b, bool ci, bool *co);
uint   sbb(uint a, uint b, bool bi, bool *bo);
uint   adc_vl(uvlong a, uvlong b, bool ci, bool *co);
uint   sbb_vl(uvlong a, uvlong b, bool bi, bool *bo);
uint   adc_rl(urlong a, urlong b, bool ci, bool *co);
uint   sbb_rl(urlong a, urlong b, bool bi, bool *bo);
uint   adc_tl(utlong a, utlong b, bool ci, bool *co);
uint   sbb_tl(utlong a, utlong b, bool bi, bool *bo);

/* Other Arithmetic */
uvlong wmul(uint a, uint b);
uvlong clmul(uint a, uint b);

struct {
	uint q, r;
} divmod(uint a, uint b);

urlong wmul_vl(uvlong a, uvlong b);
urlong clmul_vl(uvlong a, uvlong b);

struct {
	uvlong q, r;
} divmod_vl(uvlong a, uvlong b);

utlong wmul_rl(urlong a, urlong b);
utlong clmul_rl(urlong a, urlong b);

struct {
	urlong q, r;
} divmod_rl(urlong a, urlong b);

struct {
	utlong l, h;
} wmul_tl(utlong a, utlong b);

struct {
	utlong l, h;
} clmul_tl(utlong a, utlong b);

struct {
	utlong q, r;
} divmod_tl(utlong a, utlong b);

/* Comparison */
int cmp_rl(rlong a, rlong b);
int cmp_tl(tlong a, tlong b);
int cmp_sr(rlong a, rlong b);
int cmp_st(tlong a, tlong b);


/* Implementation */
