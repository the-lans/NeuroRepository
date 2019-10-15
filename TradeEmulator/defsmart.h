#ifndef DEFSMART_H
#define DEFSMART_H

#include <math.h>

#define ID_NONE -1
typedef signed int number_id;

#define NFUNC_SQR(arg) ((arg)*(arg))
#define NFUNC_SQRT(arg) sqrtf(arg)
#define NFUNC_EXP(arg) expf(arg)
#define NFUNC_EXP87 6.07603E37
#define NFUNC_LN(arg) logf(arg)
#define NFUNC_SIN(arg) sinf(arg)
#define NFUNC_COS(arg) cosf(arg)
#define NFUNC_TAN(arg) tanf(arg)
#define NFUNC_ATAN(arg) atanf(arg)
#define NFUNC_TANH(arg) tanhf(arg)
#define NFUNC_EPSILON std::numeric_limits<NType>::epsilon()
#define NFUNC_LIMITS_MAX std::numeric_limits<NType>::max()

template <typename NType> NType nfunc_sqr(NType arg) {return arg * arg;}
template <typename NType> NType nfunc_sqrt(NType arg) {return sqrtf(arg);}
template <typename NType> NType nfunc_exp(NType arg) {return expf(arg);}
template <typename NType> NType nfunc_ln(NType arg) {return logf(arg);}
template <typename NType> NType nfunc_sin(NType arg) {return sinf(arg);}
template <typename NType> NType nfunc_cos(NType arg) {return cosf(arg);}
template <typename NType> NType nfunc_tan(NType arg) {return tanf(arg);}
template <typename NType> NType nfunc_atan(NType arg) {return atanf(arg);}
template <typename NType> NType nfunc_tanh(NType arg) {return tanhf(arg);}

#endif // DEFSMART_H
