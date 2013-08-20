/* stdlib.h library for large systems - small embedded systems use clibrary.c instead */
#include "interpreter.h"

#ifndef BUILTIN_MINI_STDLIB

static int ZeroValue = 0;

#ifndef NO_FP
void StdlibAtof(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    ReturnValue->Val->FP = atof(Param[0]->Val->Pointer);
}
#endif

void StdlibAtoi(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    ReturnValue->Val->Integer = atoi(Param[0]->Val->Pointer);
}

void StdlibAtol(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    ReturnValue->Val->Integer = atol(Param[0]->Val->Pointer);
}

void StdlibStrtod(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    ReturnValue->Val->FP = strtod(Param[0]->Val->Pointer, Param[1]->Val->Pointer);
}

void StdlibStrtol(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    ReturnValue->Val->Integer = strtol(Param[0]->Val->Pointer, Param[1]->Val->Pointer, Param[2]->Val->Integer);
}

void StdlibMalloc(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    ReturnValue->Val->Pointer = malloc(Param[0]->Val->Integer);
}

void StdlibCalloc(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    ReturnValue->Val->Pointer = calloc(Param[0]->Val->Integer, Param[1]->Val->Integer);
}

void StdlibRealloc(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    ReturnValue->Val->Pointer = realloc(Param[0]->Val->Pointer, Param[1]->Val->Integer);
}

void StdlibFree(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    free(Param[0]->Val->Pointer);
}

void StdlibRand(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    ReturnValue->Val->Integer = rand();
}

void StdlibSrand(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    srand(Param[0]->Val->Integer);
}

void StdlibAbort(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    ProgramFail(Parser, "abort");
}

void StdlibExit(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    PlatformExit(Param[0]->Val->Integer);
}

void StdlibGetenv(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    //ReturnValue->Val->Pointer = getenv(Param[0]->Val->Pointer);
}

void StdlibSystem(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    //ReturnValue->Val->Integer = system(Param[0]->Val->Pointer);
}

void StdlibAbs(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    ReturnValue->Val->Integer = abs(Param[0]->Val->Integer);
}

void StdlibLabs(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    ReturnValue->Val->Integer = labs(Param[0]->Val->Integer);
}

/* all stdlib.h functions */
struct LibraryFunction StdlibFunctions[] =
{
    { StdlibAtof,           "float atof(char *);" },
    { StdlibAtoi,           "int atoi(char *);" },
    { StdlibAtol,           "int atol(char *);" },
    { StdlibStrtol,         "int strtol(char *,char **,int);" },
    { StdlibMalloc,         "void *malloc(int);" },
    { StdlibCalloc,         "void *calloc(int,int);" },
    { StdlibRealloc,        "void *realloc(void *,int);" },
    { StdlibFree,           "void free(void *);" },
    { StdlibRand,           "int rand();" },
    { StdlibSrand,          "void srand(int);" },
    { StdlibAbort,          "void abort();" },
    { StdlibExit,           "void exit(int);" },
    { StdlibGetenv,         "char *getenv(char *);" },
    { StdlibAbs,            "int abs(int);" },
    { StdlibLabs,           "int labs(int);" },
    { NULL,                 ""}
};

/* creates various system-dependent definitions */
void StdlibSetupFunc(void)
{
	// stdlib
	struct LibraryFunction *plib;
	plib = &StdlibFunctions;
	plib[ 0].Func = StdlibAtof		;
	plib[ 1].Func = StdlibAtoi		;
	plib[ 2].Func = StdlibAtol		;
	plib[ 3].Func = StdlibStrtol	;
	plib[ 4].Func = StdlibMalloc	;
	plib[ 5].Func = StdlibCalloc	;
	plib[ 6].Func = StdlibRealloc	;
	plib[ 7].Func = StdlibFree		;
	plib[ 8].Func = StdlibRand		;
	plib[ 9].Func = StdlibSrand		;
	plib[10].Func = StdlibAbort		;
	plib[11].Func = StdlibExit		;
	plib[12].Func = StdlibGetenv	;
	plib[13].Func = StdlibAbs		;
	plib[14].Func = StdlibLabs		;
    /* define NULL, TRUE and FALSE */
    if (!VariableDefined(TableStrRegister("NULL")))
        VariableDefinePlatformVar(NULL, "NULL", &IntType, (union AnyValue *)&ZeroValue, FALSE);
}

#endif /* !BUILTIN_MINI_STDLIB */
