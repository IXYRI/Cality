#pragma once

/* @brief Set the universal seed for rands and hashes
 * All rand and hash functions will be (re)initialized with the provided seed. If the seed is not long enough to cover all functions, it will be repeated and truncated as needed. States are managed internally. */
void   seedfeed(arrst seed);
uvlong rand(void);          // chacha8
uvlong qrand(void);         // w1rand
uvlong hash(arrst data);    // siphash
uvlong qhash(arrst data);   // xxhash
utlong chash(arrst msg);    // BLAKE3

/* @brief Allocate memory in arena 0, the default arena */
void  *den(uvlong size);
/* @brief Allocate and zero-initialize memory for an array in arena 0 */
void  *sala(uvlong count, uvlong elmsize);
/* @brief Reallocate memory in arena 0 */
void  *resetl(void *ptr, uvlong size);
/* @brief Allocate aligned memory in arena 0 */
void  *carrel(int align, uvlong size);
/* @brief Deallocate memory in arena 0 */
void   cede(void *ptr);
/* @brief Create a new memory arena
 * @return Arena descriptor */
int    mkarena(void);
void  *aden(int arena, uvlong size);
void  *asala(int arena, uvlong count, uvlong elmsize);
void  *aresetl(int arena, void *ptr, uvlong size);
void  *acarrel(int arena, int align, uvlong size);
void   acede(int arena, void *ptr);
void   rmarena(int arena);

typedef enum xrslt
{
	xrtuvlong,
	xrtrune,
	xrtarrst,
	xrtpair,
} xrslt;

/* @brief Create a new expedient batch processor for silos */
int mkxpedt(int arena);
/* @brief Map a function over a source
 * @param src Source no.
 * @param rs Result element type
 * @param fn Function to apply, taking a src-dst pair and a pointer to additional arguments
 * @return Result source no.
 * @note The src-dst pair consists of the source element and the result destination */
int fmap(int xpedt, int src, xrslt rs, void (*fn)(pair sd, void *), void *args);
int filt(int xpedt, int src, bool (*pred)(arrst i, void *), void *args);
int sort(int xpedt, int src, int (*cmp)(void *, void *, void *), void *args);   // smooth sort
/* @brief Collect source elements into a silo of specified type */
int mold(int xpedt, int src, silotype t);
/* @brief Fold the source silo into a single value
 * @param init Initial value of the result type */
int pare(int xpedt, int src, xrslt rs, void *init, void (*red)(arrst acc, arrst i, arrst dst, void *), void *args);
/* @brief Broadcast a single value into a source */
int diffuse(int xpedt, int src, xrslt rs);
/* @brief Zip two sources with a function */
int connex(int xpedt, int srca, int srcb, xrslt rs, void (*zippr)(arrst a, arrst b, arrst dst, void *), void *args);
/* @brief Flatten source silos into a single silo */
int flatten(int xpedt, int src);
/* @brief Map a function over a source and flatten the result */
int flatmap(int xpedt, int src, xrslt rs, void (*fn)(arrst i, void (*yield)(arrst x), void *), void *args);
/* @brief Group source elements according to their equivalence classes */
int aggroup(int xpedt, int src, int (*equiv)(arrst i, void *), void *args);
/* @brief Fold the source silo into a single value, yielding all intermediate values */
int peruse(int xpedt, int src, xrslt rs, void *init, void (*red)(arrst acc, arrst i, arrst dst, void *), void *args);
/* @brief Take the first n elements from a source */
int xtake(int xpedt, int src, uvlong n);
/* @brief Drop the first n elements from a source */
int xdrop(int xpedt, int src, uvlong n);
/* @brief Check if any element in the source satisfies a condition */
int xany(int xpedt, int src, bool (*pred)(arrst i, void *), void *args);
/* @brief Check if all elements in the source satisfy a condition */
int xall(int xpedt, int src, bool (*pred)(arrst i, void *), void *args);
/* @brief Find the first element in the source that satisfies a condition */
int xfind(int xpedt, int src, bool (*pred)(arrst i, void *), void *args);
/* @brief Make every element in the source unique
 * @param resolv Function to resolve conflicting pairs */
int xuniq(int xpedt, int src, void (*resolv)(arrst k, arrst va, arrst vb, arrst rv, void *), void *args);
/* @brief Immix two sources one by one */
int ximmix(int xpedt, int srca, int srcb);
/* @brief Catenate two sources */
int catena(int xpedt, int srca, int srcb);
/* @brief Merge two sorted sources */
int xmerge(int xpedt, int srca, int srcb, int (*cmp)(void *, void *, void *), void *args);   // merge sort
/* @brief Repeat the source infinitely */
int xrecur(int xpedt, int src);
/* @brief Slide a window over the source */
int xorama(int xpedt, int src, uvlong size, uvlong stride);

typedef union xprod
{
	int    d;
	uvlong u;
	rune   r;
	arrst  a;
	pair   p;
} xprod;

/* @brief Bind concrete silos to unresolved sources and actuate the evaluation
 * Sources form a directed acyclic graph. Each x-function depends on one or more upstream sources and returns a source number, which represents a deferred computation node. Unresolved sources are placeholders for concrete silos or values that will be provided later. */
void vxact(int xpedt, xprod *out, va_list silos);

static void inline xact(int xpedt, xprod *out, ...) {
	va_list silos;
	va_start(silos, out);
	vxact(xpedt, out, silos);
	va_end(silos);
}

/* @brief Register one batch job without evaluation */
void vxpass(int xpedt, xprod *out, va_list silos);

void xpass(int xpedt, xprod *out, ...) {
	va_list silos;
	va_start(silos, out);
	vxpass(xpedt, out, silos);
	va_end(silos);
}

/* @brief Register one batch job and a callback function for the result */
void vxcall(int xpedt, void (*fn)(xprod *out, void *), void *args, va_list silos);

void xcall(int xpedt, void (*fn)(xprod *out, void *), void *args, ...) {
	va_list silos;
	va_start(silos, args);
	vxcall(xpedt, fn, args, silos);
	va_end(silos);
}

/* @brief Register one batch job and a callback function for each element of the result */
void vxcalleach(int xpedt, void (*fn)(xprod *out, void *), void *args, va_list silos);

void xcalleach(int xpedt, void (*fn)(xprod *out, void *), void *args, ...) {
	va_list silos;
	va_start(silos, args);
	vxcalleach(xpedt, fn, args, silos);
	va_end(silos);
}

/* @brief Execute all registered batch jobs */
void xrun(int xpedt);
void rmxpedt(int xpedt);

// non thread-safe, and do parallel strategy

typedef struct rwx {
	bool r:1;
	bool w:1;
	bool x:1; /* walk permission for directories */
} rwx;

typedef struct accs {
	uchar other:3;
	uchar group:3;
	uchar owner:3;
} accs;

typedef struct perm {
	ushort accs :9;
	bool   isdir:1;
	bool   islnk:1;
} perm;

typedef struct omode {
	bool r:1;
	bool w:1;
	bool x:1; /* result to an error if the file already exists for create() */
	bool t:1; /* truncate to zero before opening */
	bool d:1; /* remove the file after closing */
	bool a:1; /* append-only */
} omode;

/* @brief Open a file with the specified mode
 * @return File descriptor
 * The file descriptor of value 0, 1, 2 is reserved for stdin, stdout, and stderr
 * @note Check errmsg() for errors */
int    dopen(char *file, omode mod);   // use union to type pun
int    dcreate(char *file, omode mod, perm pm);
/* @brief Read length bytes of data from the offset in the file associated with fd into memory at buf
 * It is not guaranteed that all length bytes will be read; for example if the file refers to the console, at most one line will be returned
 * @return The number of bytes read
 * @retval 0 End of file */
uvlong dread(int fd, arrst buf);
/* @brief Call read() successively until length bytes have been read, or an error occurs */
uvlong dreadn(int fd, arrst buf);
/* @brief Write length bytes of data from buf to the file associated with fd at the file offset */
void   dwrite(int fd, arrst buf);
/* @brief Print formatted output to the file
 * @return The number of bytes written */
uvlong dvprint(int fd, char *fm, va_list args);   // fmts
/* @brief Set the offset for the file
 * @param amount Goes to the right if positive and the left if negative regardless of from
 * @param from 0 from the start of the file, 1 from the current position, 2 from the end
 * @return New file offset value */
uvlong dseek(int fd, vlong amount, int from);
uvlong dprint(int fd, char *fm, ...);
/* @brief Read length bytes of data from the offset */
uvlong pread(int fd, arrst buf, uvlong offset);
/* @brief Write length bytes of data at the offset */
void   pwrite(int fd, arrst buf, uvlong offset);
void   dclose(int fd);

/* @brief Create a new buffered I/O in a specific arena */
int    mkbio(int arena);
/* @brief Initialize a buffered I/O with a file descriptor */
void   binit(int bd, int fd, omode mod);
/* @brief Create a new buffered I/O and open a file */
int    bopen(int arena, char *file, omode mod);
/* @return File descriptor associated with the buffer */
uvlong bfildes(int bd);
/* @brief Read a string from the file up to and including the first delim character
 * The delimiter character at the end of the line is not altered
 * @return Array struct of a pointer to the start of the line in the buffer and its length, including the delimiter */
arrst  brdline(int bd, rune delim);
/* @brief Create a strand containing the next line of input delimited by delim, terminated by a NUL byte
 * Unlike brdline(), which returns when its buffer is full even if no delimiter has been found, brdstr() will return an arbitrarily long line in a single call. If nulldelim is set, the terminal delimiter will be overwritten with a NUL, and thus not included in the result.
 * @return Strand descriptor */
int    brdstr(int bd, int arena, rune delim, bool nulldelim);
/* @brief Get the next character from the buffer */
char   bgetc(int bd);
/* @brief Called immediately after bgetc() to allow the same character to be reread */
void   bungetc(int bd);
rune   bgetrune(int bd);
/* @brief Called immediately after bgetrune() to allow the same UTF sequence to be reread as either bytes or a rune
 * @note bungetc() and bungetrune() may back up a maximum of five bytes */
void   bungetrune(int bd);
/* @brief Read the formatted floating-point number from the buffered input, skipping initial blanks and tabs */
double bgetd(int bd);
uvlong bread(int bd, arrst buf);
uvlong bseek(int bd, vlong amount, int from);
/* @return File offset of the next character to be processed */
uvlong boffset(int bd);
void   bputc(int bd, char c);
void   bputrune(int bd, rune r);
uvlong bvprint(int bd, char *fm, va_list args);

uvlong bprint(int bd, char *fm, ...) {
	va_list args;
	va_start(args, fm);
	bvprint(bd, fm, args);
	va_end(args);
}

void   bwrite(int bd, arrst buf);
/* @brief Cause any buffered output to be written
 * bflush() is called on exit for every buffer still open for writing */
void   bflush(int bd);
/* @return The number of bytes in the buffer (available for reading or ready to be written) */
uvlong bbuffered(int bd);
/* @brief Close the file or disassociate the fd */
void   bclose(int bd);
/* @brief Remove a buffered I/O, closing the file in case it was opened by the bio */
void   rmbio(int bd);

/* @brief Get or set own name */
char  *autonym(char *rename);
/* @brief Create a new collette with the given name, or null for an anonymous one
 * Collettes are independent cooperative executors for concurrent tasks, driven by a n: m scheduler running on a local multi-threaded pool. Concurrent is called collateral in the sense. At any time, there are at least three collettes: the main collette whose name is to be set and who always takes up an entire physical thread, the butler collette who is in charge of scheduling tasks, and one or more maid collettes who single-mindedly execute assigned tasks, given any collateral functions have once been called and the system is thus initialized. */
void   collat(char *name, int (*ego)(void *), void *arg);

typedef enum tsktiming
{
	ttchance  = 0,
	ttinstant = 1,
	ttsometime,
} tsktiming;

typedef struct task {
	char     *name;
	int       (*run)(void *);
	void     *arg;
	tsktiming timing;
	void      (*callback)(void *);
	void     *cbarg;
} task;

/* @brief Delegate a task to maid collettes for non-serial execution */
void   derail(task t, struct timespec intv, bool (*cond)(void *), void *arg);
void   resched(char *tskname, struct timespec intv, bool (*cond)(void *), void *arg);
/* @brief Reset the callback for a task
 * As the scheduling model is cooperative, it is advised to split heavy tasks into smaller ones and use callbacks to continuously set successive tasks. Use of yield or sleep is thus discouraged. */
void   rstcont(char *tskname, void (*callback)(void *), void *cbarg);
void   cancel(char *tskname);
/* @brief Wait a collette to finish, even if it does not exist yet */
int    waitcol(char *name);
/* @brief Wait until timeout or condition is met, checked every interval
 * @param intv Timeout or the interval to check the condition
 * @param[out] stat Pointer to store the finish status, can be null
 * @return false if aborted */
bool   waittil(char *name, struct timespec *intv, bool (*cond)(void *), void *arg, int *stat);
/* @brief Receive a message from collettes, if any */
bool   inbox(arrst *msg);
/* @brief Obtain public sayings from everyone
 * @param[in,out] things Array struct to hold the obtained sayings of length number
 * @return The actual number of sayings obtained */
int    ambnt(arrst things);
/* @brief Send a message to a collette */
void   dmesg(char *name, arrst msg);
/* @brief Announce something publicly */
void   holler(arrst sth);
/* @brief Get or set the size of the ambient space */
uvlong ambsize(uvlong set);
// For mutexes, conditional variables, and thread-local storage, use standard library directly
// note that stdatomic.h is not formally supported by MSVC


// sequel
// conseq - Continuation awaits for the future BDFA language, using Cheney on the MTA, Forth, Conbinator, Actors, and Array Oriented Programming concepts to restore the primitive OOP by Alan Kay, according to my article "Class Are Not Data Structures"
