#pragma once

/* @brief Create a new stack in a specific arena
 * @param arena Arena in which to create the stack, 0 for the default
 * @return Stack descriptor */
int    mkstack(int arena);
void   push(int stack, uvlong data);
uvlong pop(int stack);
/* @brief Peek at the top of a stack without popping it */
uvlong peep(int stack);
void   rmstack(int stack);

#define __VA_LEN__                                                                                           \
	__24TH__(__VA_ARGS__, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1)
#define __24TH__(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, X, ...) X
#define args(type, ...)                    \
	__VA_LEN__, &(type []) { __VA_ARGS__ }
#define arrlen(arr)    sizeof(arr) / sizeof(arr [0])
#define dmarray(n, ca) dmarray_##n(ca)
#define dmarray_1(ca)  (ca) [0]
#define dmarray_2(ca)  (ca) [0], (ca) [1]
#define dmarray_3(ca)  (ca) [0], (ca) [1], (ca) [2]
#define dmarray_4(ca)  (ca) [0], (ca) [1], (ca) [2], (ca) [3]
#define dmarray_5(ca)  dmarray_4(ca), (ca) [4]
#define dmarray_6(ca)  dmarray_4(ca), (ca) [4], (ca) [5]
#define dmarray_7(ca)  dmarray_4(ca), (ca) [4], (ca) [5], (ca) [6]

/* Array with Length */
typedef struct {
	uvlong len;
	void  *x;
} arrst;

arrst inline mkarrst(uvlong len, void *x) { return ( arrst ) {len, x}; }

#define arrstof(type, ...) mkarrst(args(type, __VA_ARGS__))
/* @brief Convert a C array to an array struct */
#define toarrst(arr)       mkarrst(arrlen(arr), arr)
/* @brief Unpack length and data from an array struct */
#define umarrst(a)         a.len, a.x
/* @brief Unpack length and data from a C array */
#define umarray(arr)       arrlen(arr), arr
#define arrstom(x)         mkarrst(1, &x)
#define tobytes(x)         mkarrst(sizeof(x), &x)
#define deastom(type, a)   (( type [] ) a.x) [0]
int    mkseq(int arena);
/* @brief Attach data to a sequence */
void   atch(int seq, uvlong data);
/* @brief Place data at a specific position in a sequence
 * @note pos can be negative */
void   place(int seq, vlong pos, uvlong data);
/* @brief Remove and return data at a specific position in a sequence */
uvlong drop(int seq, vlong pos);
/* @brief Swap two elements in a sequence */
void   swap(int seq, vlong pa, vlong pb);
/* @brief Pick an array struct from a sequence
 * @param end End position (inclusive) */
arrst  swath(int seq, vlong start, vlong end);
void   rmseq(int seq);

int    mkqueue(int arena);
void   enqueue(int queue, uvlong data);
uvlong dequeue(int queue);
uvlong peekq(int queue);
void   rmqueue(int queue);

int    mkset(int arena);
/* @param data Byte array containing the data */
void   adds(int set, arrst data);   // qhash()
void   dels(int set, arrst data);
/* @brief Test membership in a set */
bool   mems(int set, arrst data);
/* @brief Union two sets
 * @param[in,out] setd Destination set */
void   unions(int setd, int set);
void   intxns(int setd, int set);
void   diffs(int setd, int set);
void   symmdiffs(int setd, int set);
/* @brief Check if seta is a subset of setb */
bool   subsets(int seta, int setb);
/* @brief Compute the Cartesian product of two sets
 * @param arena Arena in which to create the result, 0 for the default
 * @return Descriptor for the resulting set of pairs (addr) */
int    cartesprod(int arena, int seta, int setb);
void   rmset(int set);

typedef struct {
	arrst a;
	arrst b;
} pair;

pair inline mkpair(arrst a, arrst b) { return ( pair ) {a, b}; }

#define umpair(p) p.a, p.b
int    mkmap(int arena);
void   insert(int map, pair kv);
bool   lookup(int map, arrst k, arrst *v);
/* @brief Delete a key-value pair from the map
 * @param[out] v Pointer to store the value if the key is found, can be null
 * @return true if the key was found and deleted, false otherwise */
bool   oblit(int map, arrst k, arrst *v);
/* @brief Update the value for a given key in the map */
void   revamp(int map, arrst k, arrst v);
/* @brief Join two maps
 * @param method 0 inner, 1 outer (full), 2 self (left) */
void   conjoin(int mapd, int map, int method);
void   rmmap(int map);

/* @brief Tokenize a string by whitespace, except treating quoted text as a single token
 * @param str The string to tokenize, modified in-place
 * @param[out] arr Array to store the tokens
 * @param max Maximum number of tokens to store
 * @return The number of tokens split
 * '' becomes null string, '''' becomes a string containing a single quote */
uvlong tknize(char *str, char **arr, uvlong max);

bool inline chkrune(rune r) { return r < 0x110000 && (r < 0xd800 || r > 0xdfff); }

/* @brief Convert a Unicode rune to char sequences
 * @return The number of bytes stored */
uvlong runetochar(char *s, rune *r);
/* @brief Convert char sequences to a Unicode rune
 * @return The number of bytes used */
uvlong chartorune(rune *r, char *s);

/* @return The number of bytes required to convert r into UTF */
uvlong inline runelen(rune r) {
	return r < 0x80 ? 1 : r < 0x800 ? 2 : r < 0x10000 ? 3 : r < 0x110000 ? 4 : (errmsg("Invalid Rune"), 5);
}

/* @return The number of bytes required to convert the n runes pointed by r into UTF */
uvlong runenlen(rune *r, uvlong n);

/* @brief Check if the char sequences represent a rune aright
 * @param[out] r Pointer to store the rune if the sequences are complete, can be null */
bool inline fullrune(char *s, int n, rune *r) {
	rune t;
	return ( uchar ) s [0] < 0x80 ? (n != 1 ? false : r ? (*r = s [0], true) : true)
	     : ( uchar ) s [0] < 0xe0
	       ? (n != 2                    ? false
			   : (s [0] & 0xe0) != 0xc0 ? false
			   : (s [1] & 0xc0) != 0x80 ? false
										: !chkrune(t = ((( rune ) s [0] & 0x1f) << 6 | s [1] & 0x3f))
				 ? errmsg("Invalid Rune"),
			   false
			   : n != runelen(t) ? (errmsg("UTF Overlong"), false)
								 : r ? (*r = t, true) : true)
	     : ( uchar ) s [0] < 0xf0
	       ? (n != 3                    ? false
			   : (s [1] & 0xc0) != 0x80 ? false
			   : (s [2] & 0xc0) != 0x80
				 ? false
				 : !chkrune(t = ((( rune ) s [0] & 0xf) << 12 | (( rune ) s [1] & 0x3f) << 6 | s [2] & 0x3f))
				 ? errmsg("Invalid Rune"),
			   false
			   : n != runelen(t) ? (errmsg("UTF Overlong"), false)
								 : r ? (*r = t, true) : true)
	     : ( uchar ) s [0] < 0xf8
	       ? (n != 4                    ? false
			   : (s [1] & 0xc0) != 0x80 ? false
			   : (s [2] & 0xc0) != 0x80 ? false
			   : (s [3] & 0xc0) != 0x80
				 ? false
				 : !chkrune(
					 t
					 = ((( rune ) s [0] & 0x7) << 18 | (( rune ) s [1] & 0x3f) << 12 | (( rune ) s [2] & 0x3f) << 6 | s [3] & 0x3f)
				   )
				 ? errmsg("Invalid Rune"),
			   false
			   : n != runelen(t) ? (errmsg("UTF Overlong"), false)
								 : r ? (*r = t, true) : true)
	       : false;
}

/* @brief Copy char sequences until a null sequence has been copied, but writes no sequences beyond ed
 * @return A pointer to a null sequence terminating d if any sequences are copied, otherwise original d */
char  *strecpy(char *d, char *ed, char *s);
char  *strecat(char *d, char *ed, char *s);
/* @return The number of runes represented by the UTF string s */
uvlong utflen(char *s);
/* @return The number of complete runes represented by the first n bytes of UTF string s */
uvlong utfnlen(char *s, uvlong n);
/* @return A pointer to the first occurrence of rune r in UTF string s, or null if r does not occur */
char  *utfrune(char *s, rune r);
char  *utfrrune(char *s, rune r);
/* @return A pointer to the first occurrence of the substring t in UTF string s, or null if there is none */
char  *utfutf(char *s, char *t);   // two-way search

/* Does not warrant NUL-termination
 * @note Can be easily cast to char * */
typedef struct {
	char  *s;
	uvlong len;
} slitr;

slitr inline mkslitr(char *s, uvlong len) { return ( slitr ) {s, len}; }

#define umslitr(s) s.s, s.len

slitr inline subslitr(char *s, vlong start, vlong end) { return ( slitr ) {s + start, end - start + 1}; }

rune  nth(slitr s, vlong n);
/* Dynamic String */
int   mkstrand(int arena);
slitr obslitr(int strand);
void  concat(int strand, char *s);
void  putstr(int strand, vlong pos, char *s);
void  dropstr(int strand, vlong pos, vlong len);
void  repeat(int strand, uvlong n);
/* @detail Structural Regular Expressions
 * Commands:
 * x/reg/cmd: Extract. For each match, do cmd.
 * y/reg/cmd: Complement Extract. Extract text in between matches.
 * g/reg/cmd: Guard. If matches, do cmd.
 * v/reg/cmd: Reverse Guard. if does not match, do cmd.
 * s/reg/text/cmd: Substitute. Replace matches with text, then do cmd on the replaced text. In the text and following commands, & refers to the whole match, and \1, \2, etc. refer to captured groups.
 * other cmd - p (keep), d (delete), c text (change), a text (append), i text (insert).
 * These commands can be nested, forming powerful structural descriptions.
 * The Regex:
 * . - any single rune except newline
 * ^ - start of string, $ - end of string, ? - optional, * - zero or more, + - one or more.
 * [set] - character class, where set can contain any characters or the above shorthands, and can be negated with ^ as the first character. Ranges are supported with -.
 * (reg) - grouping, | - alternation, which can be nested.
 * \e - escape characters and special sequences, e.g. \n, \t, \r, \\, \/, \&, \1, etc.
 * %d - digit, %a - letter, %w - alphanumeric, %s - space, %p - punctuation, %c - control, %x - hex, %l - lowercase, %u - uppercase, %g - printable, and their uppercase counterparts for complements.
 * @n', @n+, @m-, @n~m', @n^m; - quantifiers for the preceding element, specifying exact, more, less, between, and outside repetitions respectively.
 * The /reg/ always does the leftmost-longest match in linear time without non-greedy support.
 * Examples:
 * `x/.*\n/ g/string/p` - Keep lines containing "string".
 * `x/.*\n/ g/rob/ v/robot/p` - Keep lines containing "rob" but not "robot".
 * `x/%a+/ g/i/ v/../ c/I/` - Capitalize standalone "i" but not "i" in "this", "it", etc.
 * `x/{[^}]*}/ s/move\(([^,]+),([^)]+)\)/move(\2,\1)/` - Swap arguments in move function calls within braces.
 * @note Leftmost refers to the first match point, not the first pattern in alternation. e.g. /bcd|a|ab/ is the same as /ab|a|bcd/ for the string "abcde", since both will match "ab" regardless of the order of patterns. */
void  sttregex(int strand, char *se);   // two-way search & Thompson NFA / Plan 9's VM
void  rmstrand(int strand);
int   catstr(int arena, ...);
/* @return The descriptor for the char strand */
int   runetostr(int arena, rune *r);
/* @return The descriptor for the rune sequence */
int   strtorune(int arena, char *s);
#define vargs(...) vargs_(0, ##__VA_ARGS__)

va_list inline vargs_(int dummy, ...) {
	va_list args;
	va_start(args, dummy);
	return args;
}

typedef struct fmt {
	rune    r;  /* % format rune */
	int     sd; /* strand descriptor */
	uint    width;
	uint    prec;
	uint    flags;
	va_list args;
} fmt;

enum
{
	fmtwidth    = 1, /* whether width is specified */
	fmtprec     = 2, /* whether precision is specified */
	fmtzero     = 4,
	fmtshort    = 8,
	fmtbyte     = 16,
	fmtlong     = 32,
	fmtvlong    = 64,
	fmtrlong    = fmtvlong << 1,
	fmttlong    = fmtrlong << 1,
	fmtunsigned = fmttlong << 1,
	fmtspace    = fmtunsigned << 1,
	fmtplus     = fmtspace << 1,
	fmtleft     = fmtplus << 1,
	fmtcomma    = fmtleft << 1,
	fmtcomma4   = fmtcomma << 1,
	fmtalt      = fmtcomma4 << 1,
};

/* @detail Format string with the given format and arguments
 * Conversion Specification: %[flags]verb. The verb is a single character and each flag is a single character or a (decimal) numberic string. Up to two numberic strings may be used; the first is called width, the second precision. A period can be used to separate them, and if the period is present then width and precision are taken to be zero if missing, otherwise they are 'omitted'. Either or both of the numbers may be replaced with the character *, meaning that the actual number will be obtained from the argument list as an integer.
 * Numeric Verbs: d, o, b, x, and X. each interprets the flags 0, h, hh, l, ll, L, LL, u,  , +, -, ,, ;, and # to mean pad with zeros, short, byte, long, vlong, rlong, tlong, unsigned, space for plus sign, always print a sign, left justified, commas every three digits, four digits, and alternate format. The argument defaults to int. If unsigned is specified, then the argument is interpreted as a positive number and no sign is output. If precision is not omitted, the number is padded on the left with zeros until at least precision digits appear. If precision is explicitly 0, and the number is 0, no digits are generated, and alternate formatting does not apply. Then, if alternate format is specified, the number is preceded by 0o, 0x, 0X for o, x, X conversion respectively. Finally, if width is not omitted, the number is padded on the left (or right, if left justified) with enough blanks to make the field at least width characters long.
 * Floating Point Verbs: f, e, E, g, and G take a double argument. Each interprets the flags 0, l,  , +, -, and # to mean pad with zeros, long double, space for plus sign, always print a sign, and alternate format. Precision is the number of digits that are converted after the decimal place for e, E, and f conversions, and is the maximum number of significant digits for g, and G conversions. The f verb produces output of the form [-]digits[.digits]. E conversion appends an exponent E[-]digits, and e conversion appends an exponent e[-]digits. The g verb will output the argument in either e or f with the goal of producing the smallest output. Also, trailing zeros are omitted from the fraction part of the output, and a trailing decimal point appears only if it is followed by a digit. The G verb uses E format instead of e. When alternate format is specified, the result will always contain a decimal point, and for g and G conversions, tailing zeros are not removed.
 * The s verb copies a NUL-terminated string (pointer to char) to the output. The number of characters copied (n) is the minimum of the size of the string and precision. These n characters are justified within a field of width characters as described above. If a precision is given, it is safe for the string not to be NUL-terminated as long as it is at least precision characters (not bytes!) long. The S verb interprets its pointer as an array of runes; the runes are converted to UTF before output.
 * The c verb copies a single char (promoted to int) justified within a field of width characters as described above. The C verb works on runes.
 * The p verb formats a pointer value. The r verb takes no arguments; it copies the error string returned by a call to errmsg().
 * The a verb takes an array and a format string, applying the format to each or multiple elements of the array. It interprets the flags 0, h, hh, l, ll, L, LL, u, ,, -, ;, and # to mean pad with zeros, array of short, byte, long, vlong, rlong, tlong, unsigned, floating type, left justified, array struct, and compound data type. The width and precision specify the field width and the number of elements, and if ; is not specified (thus a C array), precision cannot be omitted. The floating type flag can be used with h or l to interpret the argument as an array of float or long double. If # is specified, the verb takes an additional element size argument from the list to calculate offsets after each invocation. The a verb can be used to format arrays of any type, including user-defined types with custom verbs, and can be used recursively to format nested arrays. The format string can contain multiple verbs and dynamic width and precision specifications, in which case the elements will be applied in order, and the arguments follows the format string (or element size) in the list, which will be shared across invocations in the formatting process. E.g. `fmts(0, "{ %.4a }", arr, "%d, ")` will generate a strand like "{ 1, 2, 3, 4, }" for an int[4] array. For trailing commas, `fmts(0, "{ %.3a, %d }", arr, "%d, ", arr[3])`. For dynamic arrays, `fmts(0, "[ %.*a]", len, arr, "%d ")` or `fmts(0, "[ %;a]", arrst, "%d ")`. For dynamic width/precision in the format string, `fmts(0, "< %.*,a>", len, arr, "%.*f ", prec)`. For multiple verbs, `fmts(0, "(%.2ua, %ud => %ud)", arr6, "%ud => %ud, ", arr6[4], arr6[5])`. For custom verbs, `fmts(0, "%.4#a", arri, "%I", sizeof(itype))`. For nested arrays, `fmts(0, "%.3#a", arr9, "| %.3a|\n", sizeof(arr9[0]), "%d ")` will generate "| 1 2 3 |\n| 4 5 6 |\n| 7 8 9 |\n" for an int[3][3] array.
 * Custom Verbs may be installed using fmtinstall(). */
int vfmts(int arena, char *fm, va_list args);

int inline fmts(int arena, char *fm, ...) {
	va_list args;
	va_start(args, fm);
	int res = vfmts(arena, fm, args);
	va_end(args);
	return res;
}

bool   dofmt(fmt *fp);
/* @brief Install a custom verb/flag for formatting
 * @note Arguments in variadic list undergo default argument promotions, that is, float is promoted to double, and types narrower than int are promoted to int or unsigned int. It is advised for compound types to be passed as pointers so as to work with the a verb. The installed function should return zero if the format rune is a verb after it has done formatting, or one if is a flag that needs to be set in fp->flags for the next verb. */
void   fmtinstall(rune c, bool (*fn)(fmt *));

int    mkmultiset(int arena);
void   addms(int ms, arrst data);
/* @brief Delete single instance */
void   delms(int ms, arrst data);
/* @brief Remove all occurrences */
void   prgms(int ms, arrst data);
bool   memms(int ms, arrst data);
/* @brief Count occurrences of specific data */
uvlong cntms(int ms, arrst data);
/* @brief Combine multiplicities */
void   addtums(int dms, int ms);
/* @brief Max multiplicity */
void   unionms(int dms, int ms);
/* @brief Min multiplicity */
void   intxnms(int dms, int ms);
/* @brief Subtract multiplicity */
void   diffms(int dms, int ms);
void   symmdiffms(int dms, int ms);
/* @brief Check if every element of A appears in B with at least the same multiplicity */
bool   submultisets(int msa, int msb);
/* @brief Check if A is similar to B's submultiset with a certain deviation
 * @param deviat The allowed deviation in multiplicity for each element, -1 for any
 * @param excess The total multiplicity in A that exceeds that in B, -1 for any */
bool   simsubmss(int msa, int msb, int deviat, int excess);
int    cartprodms(int arena, int seta, int setb);
void   rmmultiset(int ms);

/* @return The size of the silo */
uvlong cize(int desc);
/* @brief Reserve space in the silo */
void   efflate(int desc, uvlong size);
/* @brief Shrink the silo to its current size */
void   tamp(int desc);
/* @brief Swap data with the silo
 * @param[in,out] data Pointer to uvlong for stack/seq/queue, rune for strand, arrst for set/multiset, or pair for map
 * Stack - Swap with the top element
 * Sequence - Swap with the last element
 * Queue - Enqueue and dequeue element
 * Set - Swap with an arbitrary element
 * Map - Swap with an arbitrary pair
 * Strand - Swap with the last rune
 * Multiset - Swap with an arbitrary element */
void   swop(int desc, void *data);
/* @brief Add an array of elements to the silo
 * @param data Array struct of uvlong, rune, arrst, or pair */
void   cram(int desc, arrst data);
/* @brief Take an array of elements from the silo
 * @param[in,out] data Array struct to determine the number of elements to take and store */
void   spew(int desc, arrst data);
/* @brief Empty the silo */
void   teem(int desc);
/* @brief Create a replica of the silo
 * @note Shallow copy, as silos don't own their elements */
int    replica(int arena);

/* Iterators
 * 1) Modification Is Invalidation
 * 2) No Warrant for Iteration Path */
int    mkiter(int desc);
/* @brief Create an obliterating iterator for the silo
 * @return Iterator descriptor */
int    mkoter(int desc);
/* @brief Obtain the next element from the iterator
 * @param data Pointer to uvlong, rune, arrst, or pair
 * @return true if an element was obtained, false otherwise */
bool   next(int iter, void *data);
void   rmiter(int iter);

typedef enum silotype
{
	stackt = 1,
	seqt,
	queuet,
	sett,
	mapt,
	strandt,
	multisett,
} silotype;

// All silos are not thread-safe
// type information in silo descriptor
// No progressive resizing for the 8-byte (or 16- 32-byte) elements. If data exceeds 8 MB, mount new chunks.
