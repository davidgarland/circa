#include "cx32.h"

#define GEN_UNARY(F, O) static inline C2(T, x32) C3(T, x32, F)(C2(T, x32) v) { return circa_gnu_or_std(O v, (C2(T, x32)) {O v.V0, O V.V1, O v.V2, O v.V3, O v.V4, O v.V5, O v.V6, O v.V7, O v.V8, O v.V9, O v.V10, O v.V11, O v.V12, O v.V13, O v.V14, O v.V15, O v.V16, O v.V17, O v.V18, O v.V19, O v.V20, O v.V21, O v.V22, O v.V23, O v.V24, O v.V25, O v.V26, O v.V27, O v.V28, O v.V29, O v.V30, O v.V31}); }
  GEN_UNARY(not, ~);
#undef GEN_UNARY

#define GEN_OP(F, O) static inline C2(T, x32) C3(T, x32, F)(C2(T, x32) a, C2(T, x32) b) { return circa_gnu_or_std(a O b, (C2(T, x32)) { a.V0 O b.V0, a.V1 O b.V1, a.V2 O b.V2, a.V3 O b.V3, a.V4 O b.V4, a.V5 O b.V5, a.V6 O b.V6, a.V7 O b.V7, a.V8 O b.V8, a.V9 O b.V9, a.V10 O b.V10, a.V11 O b.V11, a.V12 O b.V12, a.V13 O b.V13, a.V14 O b.V14, a.V15 O b.V15, a.V16 O b.V16, a.V17 O b.V17, a.V18 O b.V18, a.V19 O b.V19, a.V20 O b.V20, a.V21 O b.V21, a.V22 O b.V22, a.V23 O b.V23, a.V24 O b.V24, a.V25 O b.V25, a.V26 O b.V26, a.V27 O b.V27, a.V28 O b.V28, a.V29 O b.V29, a.V30 O b.V30, a.V31 O b.V31 }); }
  GEN_OP(and, &)
  GEN_OP( or, |)
  GEN_OP(xor, ^)
  GEN_OP(mod, %)
  GEN_OP(lsh, <<)
  GEN_OP(rsh, >>)
#undef GEN_OP

#define GEN_OP1(F, B) static inline C2(T, x32) C3(T, x32, F)(C2(T, x32) v, T f) { return C3(T, x32, B)(v, C2(T, x32_set1)(f)); }
  GEN_OP1(and1, and)
  GEN_OP1( or1,  or)
  GEN_OP1(xor1, xor)
  GEN_OP1(mod1, mod)
  GEN_OP1(lsh1, lsh)
  GEN_OP1(rsh1, rsh)
#undef GEN_OP1

#define GEN_1OP(F, B) static inline C2(T, x32) C3(T, x32, F)(T f, C2(T, x32) v) { return C3(T, x32, B)(C2(T, x32_set1)(f), v); }
  GEN_1OP(1mod, mod)
  GEN_1OP(1lsh, lsh)
  GEN_1OP(1rsh, rsh)
#undef GEN_1OP