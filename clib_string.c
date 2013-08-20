/* string.h library for large systems - small embedded systems use clibrary.c instead */
#include "interpreter.h"

#ifndef BUILTIN_MINI_STDLIB

static int ZeroValue = 0;

void StringStrcpy(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    ReturnValue->Val->Pointer = strcpy(Param[0]->Val->Pointer, Param[1]->Val->Pointer);
}

void StringStrncpy(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    ReturnValue->Val->Pointer = strncpy(Param[0]->Val->Pointer, Param[1]->Val->Pointer, Param[2]->Val->Integer);
}

void StringStrcmp(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    ReturnValue->Val->Integer = strcmp(Param[0]->Val->Pointer, Param[1]->Val->Pointer);
}

void StringStrncmp(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    ReturnValue->Val->Integer = strncmp(Param[0]->Val->Pointer, Param[1]->Val->Pointer, Param[2]->Val->Integer);
}

void StringStrcat(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    ReturnValue->Val->Pointer = strcat(Param[0]->Val->Pointer, Param[1]->Val->Pointer);
}

void StringStrncat(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    ReturnValue->Val->Pointer = strncat(Param[0]->Val->Pointer, Param[1]->Val->Pointer, Param[2]->Val->Integer);
}

void StringStrlen(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    ReturnValue->Val->Integer = strlen(Param[0]->Val->Pointer);
}

void StringMemset(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    ReturnValue->Val->Pointer = memset(Param[0]->Val->Pointer, Param[1]->Val->Integer, Param[2]->Val->Integer);
}

void StringMemcpy(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    ReturnValue->Val->Pointer = memcpy(Param[0]->Val->Pointer, Param[1]->Val->Pointer, Param[2]->Val->Integer);
}

void StringMemcmp(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    ReturnValue->Val->Integer = memcmp(Param[0]->Val->Pointer, Param[1]->Val->Pointer, Param[2]->Val->Integer);
}

void StringMemmove(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    ReturnValue->Val->Pointer = memmove(Param[0]->Val->Pointer, Param[1]->Val->Pointer, Param[2]->Val->Integer);
}

void StringMemchr(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    ReturnValue->Val->Pointer = memchr(Param[0]->Val->Pointer, Param[1]->Val->Integer, Param[2]->Val->Integer);
}

void StringStrchr(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    ReturnValue->Val->Pointer = strchr(Param[0]->Val->Pointer, Param[1]->Val->Integer);
}

void StringStrrchr(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    ReturnValue->Val->Pointer = strrchr(Param[0]->Val->Pointer, Param[1]->Val->Integer);
}

void StringStrstr(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    ReturnValue->Val->Pointer = strstr(Param[0]->Val->Pointer, Param[1]->Val->Pointer);
}

/* all string.h functions */
struct LibraryFunction StringFunctions[] =
{
    { StringMemcpy,        "void *memcpy(void *,void *,int);" },
    { StringMemmove,       "void *memmove(void *,void *,int);" },
    { StringMemchr,        "void *memchr(char *,int,int);" },
    { StringMemcmp,        "int memcmp(void *,void *,int);" },
    { StringMemset,        "void *memset(void *,int,int);" },
    { StringStrcat,        "char *strcat(char *,char *);" },
    { StringStrncat,       "char *strncat(char *,char *,int);" },
    { StringStrchr,        "char *strchr(char *,int);" },
    { StringStrrchr,       "char *strrchr(char *,int);" },
    { StringStrcmp,        "int strcmp(char *,char *);" },
    { StringStrncmp,       "int strncmp(char *,char *,int);" },
    { StringStrcpy,        "char *strcpy(char *,char *);" },
    { StringStrncpy,       "char *strncpy(char *,char *,int);" },
    { StringStrlen,        "int strlen(char *);" },
    { StringStrstr,        "char *strstr(char *,char *);" },
    { NULL,                "" }
};

/* creates various system-dependent definitions */
void StringSetupFunc(void)
{
	// string
	struct LibraryFunction *plib;
	// ctype
	plib = &StringFunctions;
	plib[ 0].Func = StringMemcpy	;
	plib[ 1].Func = StringMemmove	;
	plib[ 2].Func = StringMemchr	;
	plib[ 3].Func = StringMemcmp	;
	plib[ 4].Func = StringMemset	;
	plib[ 5].Func = StringStrcat	;
	plib[ 6].Func = StringStrncat	;
	plib[ 7].Func = StringStrchr	;
	plib[ 8].Func = StringStrrchr	;
	plib[ 9].Func = StringStrcmp	;
	plib[10].Func = StringStrncmp	;
	plib[11].Func = StringStrcpy	;
	plib[12].Func = StringStrncpy	;
	plib[13].Func = StringStrlen	;
	plib[14].Func = StringStrstr	;
    /* define NULL */
    if (!VariableDefined(TableStrRegister("NULL")))
        VariableDefinePlatformVar(NULL, "NULL", &IntType, (union AnyValue *)&ZeroValue, FALSE);
}

#endif /* !BUILTIN_MINI_STDLIB */
