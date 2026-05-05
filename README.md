# Cality

Cality is the seed text for Coetua: a small C library vocabulary with Plan 9 taste.

It sketches:

- portable atoms and slice types;
- descriptor-owned arenas, silos, strands, and maps;
- UTF/rune helpers and structural regular expressions;
- `fmt` formatting, raw I/O, and buffered I/O;
- topology names under `nexus`;
- xpedt-style source algebra;
- later design seeds such as collateral/concurrency, segheap, and sparsplane.

Cality is not treated as a stable API or a parity target. Some files are incomplete, some declarations outrun their implementations, and some broad machinery exists mainly as direction. Coetua carries the vocabulary forward with sharper contracts, smaller accidental surface, and tests.

For current work, read Cality as taste and archaeology. Build new code in Coetua only when a caller, a public contract, and focused tests make the seam earned.