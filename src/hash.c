#include "cality.h"
#include <string.h>

// FNV-1a as a temporary minimal siphash replacement
uvlong hash(arrst data) {
	uvlong h = 0xcbf29ce484222325ULL;
	uchar *p = (uchar *)data.x;
	for (uvlong i = 0; i < data.len; i++) {
		h ^= p[i];
		h *= 0x100000001b3ULL;
	}
	return h;
}

// DJB2 as a temporary xxhash replacement
uvlong qhash(arrst data) {
	uvlong h = 5381;
	uchar *p = (uchar *)data.x;
	for (uvlong i = 0; i < data.len; i++) {
		h = ((h << 5) + h) ^ p[i]; // h * 33 ^ c
	}
	return h;
}

// BLAKE3 placeholder
utlong chash(arrst msg) {
	uvlong h1 = hash(msg);
	uvlong h2 = qhash(msg);
	// Return a basic combined hash into utlong struct
	return (utlong){h1, h2, h1 ^ h2, h2 ^ h1};
}
