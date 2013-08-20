#include "interpreter.h"
#include "dmath.h"

static double M_EValue =        2.7182818284590452354;   /* e */
static double M_LOG2EValue =    1.4426950408889634074;   /* log_2 e */
static double M_LOG10EValue =   0.43429448190325182765;  /* log_10 e */
static double M_LN2Value =      0.69314718055994530942;  /* log_e 2 */
static double M_LN10Value =     2.30258509299404568402;  /* log_e 10 */
static double M_PIValue =       3.14159265358979323846;  /* pi */
static double M_PI_2Value =     1.57079632679489661923;  /* pi/2 */
static double M_PI_4Value =     0.78539816339744830962;  /* pi/4 */
static double M_1_PIValue =     0.31830988618379067154;  /* 1/pi */
static double M_2_PIValue =     0.63661977236758134308;  /* 2/pi */
static double M_2_SQRTPIValue = 1.12837916709551257390;  /* 2/sqrt(pi) */
static double M_SQRT2Value =    1.41421356237309504880;  /* sqrt(2) */
static double M_SQRT1_2Value =  0.70710678118654752440;  /* 1/sqrt(2) */

void MathSin(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    ReturnValue->Val->FP = sin(Param[0]->Val->FP);
}
void MathCos(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    ReturnValue->Val->FP = cos(Param[0]->Val->FP);
}
void MathTan(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    ReturnValue->Val->FP = tan(Param[0]->Val->FP);
}
void MathSqrt(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    ReturnValue->Val->FP = sqrt(Param[0]->Val->FP);
}
void MathFabs(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    ReturnValue->Val->FP = fabs(Param[0]->Val->FP);
}
void MathLn(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    ReturnValue->Val->FP = ln(Param[0]->Val->FP);
}
void MathLog(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    ReturnValue->Val->FP = log(Param[0]->Val->FP,Param[1]->Val->FP);
}
void MathExp(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    ReturnValue->Val->FP = exp(Param[0]->Val->FP);
}
void MathPow(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    ReturnValue->Val->FP = pow(Param[0]->Val->FP, Param[1]->Val->FP);
}

struct LibraryFunction MathFunctions[] =
{
	{MathSin			,"double sin(double);"},
	{MathCos			,"double cos(double);"},
	{MathTan			,"double tan(double);"},
	{MathSqrt			,"double sqrt(double);"},
	{MathFabs			,"double fabs(double);"},
	{MathLn				,"double ln(double);"},
	{MathLog			,"double log(double);"},
	{MathExp			,"double exp(double);"},
	{MathPow			,"double pow(double);"},
	{NULL,""}
};

void MathSetupFunc(void)
{
	struct LibraryFunction *plib;
	plib = MathFunctions;
	plib[ 0].Func = MathSin;
	plib[ 1].Func = MathCos;
	plib[ 2].Func = MathTan;
	plib[ 3].Func = MathSqrt;
	plib[ 4].Func = MathFabs;
	plib[ 5].Func = MathLn;
	plib[ 6].Func = MathLog;
	plib[ 7].Func = MathExp;
	plib[ 8].Func = MathPow;
	
    VariableDefinePlatformVar(NULL, "M_E", &FPType, (union AnyValue *)&M_EValue, FALSE);
    VariableDefinePlatformVar(NULL, "M_LOG2E", &FPType, (union AnyValue *)&M_LOG2EValue, FALSE);
    VariableDefinePlatformVar(NULL, "M_LOG10E", &FPType, (union AnyValue *)&M_LOG10EValue, FALSE);
    VariableDefinePlatformVar(NULL, "M_LN2", &FPType, (union AnyValue *)&M_LN2Value, FALSE);
    VariableDefinePlatformVar(NULL, "M_LN10", &FPType, (union AnyValue *)&M_LN10Value, FALSE);
    VariableDefinePlatformVar(NULL, "M_PI", &FPType, (union AnyValue *)&M_PIValue, FALSE);
    VariableDefinePlatformVar(NULL, "M_PI_2", &FPType, (union AnyValue *)&M_PI_2Value, FALSE);
    VariableDefinePlatformVar(NULL, "M_PI_4", &FPType, (union AnyValue *)&M_PI_4Value, FALSE);
    VariableDefinePlatformVar(NULL, "M_1_PI", &FPType, (union AnyValue *)&M_1_PIValue, FALSE);
    VariableDefinePlatformVar(NULL, "M_2_PI", &FPType, (union AnyValue *)&M_2_PIValue, FALSE);
    VariableDefinePlatformVar(NULL, "M_2_SQRTPI", &FPType, (union AnyValue *)&M_2_SQRTPIValue, FALSE);
    VariableDefinePlatformVar(NULL, "M_SQRT2", &FPType, (union AnyValue *)&M_SQRT2Value, FALSE);
    VariableDefinePlatformVar(NULL, "M_SQRT1_2", &FPType, (union AnyValue *)&M_SQRT1_2Value, FALSE);
}
