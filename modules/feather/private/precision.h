#ifndef INC_3D_ENGINE_PRECISION_H
#define INC_3D_ENGINE_PRECISION_H

#include "cfloat"
#include "cmath"

namespace feather {

#ifdef SINGLE_PRECISION

typedef float real;
#define REAL_MAX FLOAT_MAX
#define real_sqrt sqrtf
#define real_abs fabsf
#define real_sin sinf
#define real_cos cosf
#define real_exp expf
#define real_pow powf
#define real_fmod fmodf
#define R_PI 3.14159f

#else

typedef double real;
#define REAL_MAX DOUBLE_MAX
#define real_sqrt sqrt
#define real_abs fabs
#define real_sin sin
#define real_cos cos
#define real_exp exp
#define real_pow pow
#define real_fmod fmod
#define R_PI 3.14159265358979

#endif

}


#endif //INC_3D_ENGINE_PRECISION_H
