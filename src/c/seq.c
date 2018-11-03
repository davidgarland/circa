/*
** seq.c | The Circa Library Set | Dynamically allocated sequences.
** https://github.com/davidgarland/circa
*/

#include "../h/seq.h"

/*
** Accessors
*/

struct seq_data *seq_(Seq s, CIRCA_ARGS) {
  {
    circa_assert(s != NULL, fname, line);
  }
  return ((struct seq_data*) s) - 1;
}

Seq seq_set_(size_t siz, Seq s, size_t a, void *v, size_t ext, CIRCA_ARGS) {
  {
    circa_assert(siz > 0, fname, line);
    circa_assert(s != NULL, fname, line);
    circa_assert(v != NULL, fname, line);
  }
  s = seq_rqr_(siz, s, a + 1, ext, fname, line);
  if (a >= seq(s)->len) seq(s)->len = a + 1;
  memcpy(((char*) s) + (siz * a), v, siz);
  return s;
}

void *seq_get_(size_t siz, Seq s, size_t a, CIRCA_ARGS) {
  {
    circa_assert(siz > 0, fname, line);
    circa_assert(s != NULL, fname, line);
    circa_assert(a < seq(s)->len, fname, line);
  }
  return (void*) (((char*) s) + (siz * a));
}

/*
** Allocators
*/

Seq seq_new_(size_t siz, size_t cap, CIRCA_ARGS) {
  {
    circa_assert(siz > 0, fname, line);
    circa_assert(cap > 0, fname, line);
  }
  struct seq_data* dp = NULL;
  while (dp == NULL)
    dp = calloc(sizeof(*dp) + (cap * siz), 1);
  dp->cap = cap;
  return dp->data;
}

Seq seq_rsz_(size_t siz, Seq s, size_t cap, CIRCA_ARGS) {
  {
    circa_assert(siz > 0, fname, line);
    circa_assert(s != NULL, fname, line);
    circa_assert(cap > 0, fname, line);
    circa_assert(cap >= seq(s)->len, fname, line);
  }
  if (cap < seq(s)->cap)
    memset(((char*) s) + cap, 0, (seq(s)->cap - cap) * siz);
  struct seq_data *restrict dp = NULL;
  while (dp == NULL)
    dp = realloc(seq(s), sizeof(*dp) + (cap * siz));
  if (cap > dp->cap)
    memset(((char*) s) + ((dp->cap - 1) * siz), 0, (cap - dp->cap - 1) * siz);
  dp->cap = cap;
  return dp->data;
}

Seq seq_wrap_(size_t siz, size_t len, void *v, CIRCA_ARGS) {
  {
    circa_assert(siz > 0, fname, line);
    circa_assert(len > 0, fname, line);
    circa_assert(v != NULL, fname, line);
  }
  Seq s = seq_new_(siz, len, fname, line);
  memcpy(s, v, (len * siz));
  seq(s)->len = len;
  return s;
}

Seq seq_from_(size_t siz, Seq s, CIRCA_ARGS) {
  {
    circa_assert(siz > 0, fname, line);
    circa_assert(s != NULL, fname, line);
    circa_assert(seq(s)->len > 0, fname, line);
  }
  return seq_wrap_(siz, seq(s)->len, s, fname, line);
}

Seq seq_rqr_(size_t siz, Seq s, size_t cap, size_t ext, CIRCA_ARGS) {
  {
    circa_assert(siz > 0, fname, line);
    circa_assert(s != NULL, fname, line);
  }
  return (seq(s)->cap < cap) ? seq_rsz_(siz, s, cap + ext, fname, line) : s;
}

Seq seq_del_(size_t siz, Seq s, CIRCA_ARGS) {
  {
    circa_assert(siz > 0, fname, line);
    circa_assert(s != NULL, fname, line);
  }
  if (s != NULL) {
    memset(seq(s), 0, sizeof(*seq(s)) + seq(s)->cap * siz);
    free(seq(s));
  }
  return NULL;
}

/*
** Stack Ops
*/

_circa_rets_
Seq seq_push_(size_t siz, Seq s, void *v, size_t ext, CIRCA_ARGS) {
  {
    circa_assert(siz > 0, fname, line);
    circa_assert(s != NULL, fname, line);
    circa_assert(v != NULL, fname, line);
  }

}

void *seq_pop_(size_t siz, Seq s, size_t n, CIRCA_ARGS) {
  {
    circa_assert(siz > 0, fname, line);
    circa_assert(s != NULL, fname, line);
    circa_assert(seq(s)->len > 0, fname, line);
  }

}

/*
** Sequence Ops
*/

_circa_rets_
Seq seq_cpy_(size_t siz, Seq dst, Seq src, size_t ext, CIRCA_ARGS) {
  {
    circa_assert(siz > 0, fname, line);
    circa_assert(dst != NULL, fname, line);
    circa_assert(src != NULL, fname, line);
    circa_assert(seq(src)->len > 0, fname, line);
  }
  if (dst == src) return dst;
  const size_t len = seq(src)->len;
  dst = seq_rqr_(siz, dst, len, ext, fname, line);
  seq(dst)->len = len;
  memcpy(dst, src, len * siz);
  return dst;
}

_circa_rets_
Seq seq_cat_(size_t siz, Seq dst, Seq src, size_t ext, CIRCA_ARGS) {
  {
    circa_assert(siz > 0, fname, line);
    circa_assert(dst != NULL, fname, line);
    circa_assert(src != NULL, fname, line);
    circa_assert(seq(src)->len > 0, fname, line);
  }
  const size_t dst_len = seq(dst)->len;
  const size_t src_len = seq(src)->len;
  const size_t tot_len = dst_len + src_len;
  dst = seq_rqr_(siz, dst, tot_len, ext, fname, line);
  memcpy(((char*) dst) + (dst_len * siz), src, src_len * siz);
  seq(dst)->len = tot_len;
  return dst;
}

_circa_rets_
Seq seq_rvs_(size_t siz, Seq s, CIRCA_ARGS) {
  {
    circa_assert(siz > 0, fname, line);
    circa_assert(s != NULL, fname, line);
    circa_assert(seq(s)->len > 0, fname, line);
  }
  const size_t s_len = seq(s)->len;
  s = seq_rqr_(siz, s, s_len + 1, 0, fname, line);
  void *const restrict tmp = (((char*) s) + (siz * s_len));
  for (size_t i = 0, j = s_len - 1; i < s_len / 2; i++, j--) {
    memmove(tmp, seq_get_(siz, s, i, fname, line), siz);
    memmove(seq_get_(siz, s, i, fname, line),
            seq_get_(siz, s, j, fname, line), siz);
    memmove(seq_get_(siz, s, j, fname, line), tmp, siz);
  }
  return s;
}
