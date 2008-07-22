#include <math.h>
#include <float.h>
#include <limits.h>

#define BITS_FLT 32
#define EXC_FLT 127
#define MANT_FLT 23
#define SIGN_FLT ((floatint)1<<(BITS_FLT-1))

#define BITS_DBL 64
#define EXC_DBL 1023
#define MANT_DBL 52
#define SIGN_DBL ((doubleint)1<<(BITS_DBL-1))

#if ULONG_MAX/2+1 == (1UL << (BITS_FLT-1)) 
typedef unsigned long int floatint;
#elif UINT_MAX/2+1 == (1U << (BITS_FLT-1))
typedef unsigned int floatint;

#else
#error "no integral type with BITS_FLT bits"
#endif

#if ULONG_MAX/2+1 == (1UL << (BITS_DBL-1))
typedef unsigned long int doubleint;
#else
/* We cannot portably test for the existance of long long.
 * If it doesn't exist, either the compiler or sanitychecks
 * will complain
 */
typedef unsigned long int doubleint;
#endif

float clearbit (float f, int bit);
float setbit (float f, int bit);
float xorbit (float f, int bit, int b);
float andbit (float f, int bit, int b);
float orbit (float f, int bit, int b);