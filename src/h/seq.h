/*
** seq.h | The Circa Library Set | Dynamic Sequences
** https://github.com/davidgarland/circa
*/

#ifndef CIRCA_SEQ_H
#define CIRCA_SEQ_H

#include "core.h"

/*
** Dynamic sequences work by using a style of "fat pointers" where the data is
** stored in an address that preceeds the data stored by the array portion.
** This can be seen below; there is `cap` for capacity, `len` for the length,
** and `data` as a flexible array member to be used for the type-generic array.
*/

struct seq_data {
  size_t cap;
  size_t len;
  char data[];
};

/*
** Because macros are run during preprocessing, they actually don't take up any
** namespace as far as variables and types are concerned. This means we can have
** a type named `Seq` and a macro named `Seq(T)` without any conflict; only if
** it sees parenthesis will it be checked as a macro.
*/

#define Seq(T) T*
typedef Seq(void) Seq;

/*
** Now we define our accessor functions. Note how the raw forms of the functions
** have a trailing underscore-- this will become a recurring pattern. The macros
** exist to make the functions more ergonomic, avoiding the need for idioms such
** as setting the sequence to the return value of the function, like you might
** see in a library such as Antirez's SDS. Additionally, you'll see there are
** `_iso` variants of functions-- these are for ISO-compliant standard C with
** no GNU extensions.
*/

static inline struct seq_data *seq(Seq s);

#define seq_clear_iso(T, S) seq_clear_(sizeof(T), (S))
#define seq_clear(S) seq_clear_iso(typeof(*S), S)
void seq_clear_(size_t siz, Seq s);

#define seq_set_lit_iso(T, S, A, V) (S) = seq_set_(sizeof(T), (S), (A), &(T){V})
#define seq_set_lit(S, A, V) seq_set_lit_iso(typeof(*S), S, A, V)
#define seq_set_iso(T, S, A, V) (S) = seq_set_(sizeof(T), (S), (A), &(V))
#define seq_set(S, A, V) seq_set_iso(typeof(*S), S, A, V)
Seq seq_set_(size_t siz, Seq s, size_t a, void *v);

#define seq_has_iso(T, S, A) seq_has_(sizeof(T), (S), (A))
#define seq_has(S, A) seq_has_iso(typeof(*S), S, A)
bool seq_has_(size_t siz, Seq s, size_t a);

#define seq_get_iso(T, S, A) (*((T*) seq_get_(sizeof(T), (S), (A))))
#define seq_get(S, A) seq_get_iso(typeof(*S), S, A)
void *seq_get_(size_t siz, Seq s, size_t a);

#define seq_alloc_iso(T, C) seq_alloc_(sizeof(T), C)
#define seq_alloc(T, C) seq_alloc_iso(T, C)
Seq seq_alloc_(size_t siz, size_t cap);

#define seq_realloc_iso(T, S, C) (S) = seq_realloc_(sizeof(T), (S), (C))
#define seq_realloc(S, C) seq_realloc_iso(typeof(*S), S, C)
Seq seq_realloc_(size_t siz, Seq s, size_t cap);

#define seq_require_iso(T, S, C) (S) = seq_require_(sizeof(T), (S), (C))
#define seq_require(S, C) seq_require_iso(typeof(*S), S, C)
Seq seq_require_(size_t siz, Seq s, size_t cap);

#define seq_wrap_iso(T, N, V) seq_wrap_(sizeof(T), (N), (V))
#define seq_wrap(N, V) seq_wrap_iso(typeof(*V), N, V)
Seq seq_wrap_(size_t siz, size_t n, void *v);

#define seq_free(S) (S) = seq_free_((S))
Seq seq_free_(Seq s);

#define seq_push_lit_iso(T, S, V) (S) = seq_push_(sizeof(T), (S), &(T){V})
#define seq_push_lit(S, V) seq_push_lit_iso(typeof(*S), S, V)
#define seq_push_iso(T, S, V) (S) = seq_push_(sizeof(T), (S), &(V))
#define seq_push(S, V) seq_push_iso(typeof(*S), S, V)
Seq seq_push_(size_t siz, Seq s, void *v);

#define seq_tos_iso(T, S) (*((T*) seq_pop_(sizeof(T), (S), 0)))
#define seq_tos(S) seq_tos_iso(typeof(*S), S)
#define seq_pop_iso(T, S) (*((T*) seq_pop_(sizeof(T), (S), 1)))
#define seq_pop(S) seq_pop_iso(typeof(*S), S)
void *seq_pop_(size_t siz, Seq s, size_t n);

/*
** Finally, there are some static function implementations.
*/

static inline
struct seq_data *seq(Seq s) {
  return s ? ((struct seq_data*) s) - 1 : s;
}

/*
** And now we can make some nice iteration macros.
*/

/* Translatory */

// TODO: seq_require() for A. use variable for F(...), to allow structs.
#define seq_map_iso(T, A, F, B)                  \
do {                                             \
  seq(A)->len = 0;                               \
  for (size_t I = 0; I < seq(B)->len; I++)       \
    seq_push_lit_iso(T, A, F(seq_get_iso(T, B, I))); \
} while (0)
#define seq_map(A, F, B) seq_map_iso(typeof(*B), A, F, B)

#define seq_filter_iso(T, A, F, B)              \
do {                                            \
  seq(A)->len = 0;                              \
  for (size_t I = 0; I < seq(B)->len; I++)      \
    if (F(seq_get_iso(T, B, I)))                \
      seq_push_lit_iso(T, A, seq_get_iso(T, B, I)); \
} while (0)
#define seq_filter(A, F, B) seq_filter_iso(typeof(*B), A, F, B)

#define seq_reduce_iso(T, A, F, B)         \
do {                                       \
  for (size_t I = 0; I < seq(B)->len; I++) \
    A = F(A, seq_get_iso(T, B, I));        \
} while (0)
#define seq_reduce(A, F, B) seq_reduce_iso(typeof(*B), A, F, B)

/* In-Place */

#define seq_apply_iso(T, S, F)                     \
do {                                               \
  for (size_t I = 0; I < seq(S)->len; I++)         \
    seq_set_lit_iso(T, S, I, F(seq_get_iso(T, S, I))); \
} while (0)
#define seq_apply(S, F) seq_apply_iso(typeof(*S), S, F)

#define seq_keep_iso(T, S, F)                                        \
do {                                                                 \
  size_t LEN = seq(S)->len;                                          \
  for (size_t I = 0; I < LEN; I++)                                   \
    if (F(seq_get_iso(T, S, I)))                                     \
      seq_push_lit_iso(T, S, seq_get_iso(T, S, I));                      \
  seq(S)->len -= LEN;                                                \
  memcpy(S, ((char*) S) + LEN * sizeof(T), seq(S)->len * sizeof(T)); \
} while (0)
#define seq_keep(S, F) seq_keep_iso(typeof(*S), S, F)

#define seq_collapse_iso(T, S, F) \
do {                              \
  T VAL = {0};                    \
  seq_reduce_iso(T, VAL, F, S);   \
  seq(S)->len = 1;                \
  seq_set_iso(T, S, 0, VAL);      \
} while (0)
#define seq_collapse(S, F) seq_collapse_iso(typeof(*S), S, F)

/* Side Effectful */

#define seq_do_iso(T, S, F)                \
do {                                       \
  for (size_t I = 0; I < seq(S)->len; I++) \
    F(seq_get_iso(T, S, I));               \
} while (0)
#define seq_do(S, F) seq_do_iso(typeof(*S), S, F)

#endif /* CIRCA_SEQ_H */
