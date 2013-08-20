/* stdio.h library for large systems - small embedded systems use clibrary.c instead */
//#include <errno.h>
#include <nspireio2.h>

#include "interpreter.h"
#include "dconsole.h"

static int ZeroValue = 0;
static int EOFValue = EOF;
static int SEEK_SETValue = SEEK_SET;
static int SEEK_CURValue = SEEK_CUR;
static int SEEK_ENDValue = SEEK_END;
static int BUFSIZValue = BUFSIZ;
static int FILENAME_MAXValue = FILENAME_MAX;

int StrPutChar(char * dest,char c)
{
	*dest++ = c;
	*dest = '\0';
	return 1;
}

int StrPutStr(char * dest,const char * scr)
{
	int l;
	for (l=0;*scr;++l)
		*dest++ = *scr++;
	*dest = '\0';
	return l;
}

void GenericPrintf(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs, char * str)
{
    const char *FPos;
    struct Value *NextArg = Param[0];
    struct ValueType *FormatType;
    int ArgCount = 1;
    int LeftJustify = FALSE;
    int ZeroPad = FALSE;
    int FieldWidth = 0;
    const char *Format = Param[0]->Val->Pointer;
    
	int l;
	
    for (FPos = Format; *FPos != '\0'; FPos++)
    {
        if (*FPos == '%')
        {
            FPos++;
            if (*FPos == '-')
            {
                /* a leading '-' means left justify */
                LeftJustify = TRUE;
                FPos++;
            }
            
            if (*FPos == '0')
            {
                /* a leading zero means zero pad a decimal number */
                ZeroPad = TRUE;
                FPos++;
            }
            
            /* get any field width in the format */
            while (isdigit((int)*FPos))
                FieldWidth = FieldWidth * 10 + (*FPos++ - '0');
            
            /* now check the format type */
            switch (*FPos)
            {
                case 's': FormatType = CharPtrType; break;
                case 'd': case 'u': case 'x': case 'b': case 'c': FormatType = &IntType; break;
                case 'f': FormatType = &FPType; break;
                case '%': StrPutChar(str,'%');++str;FormatType = NULL; break;
                case '\0': FPos--; FormatType = NULL; break;
                default:  StrPutChar(str,*FPos);++str; FormatType = NULL; break;
            }
            
            if (FormatType != NULL)
            { 
                /* we have to format something */
                if (ArgCount >= NumArgs)
				{
					l = StrPutStr(str,"XXX");str+=l;
				}
                else
                {
                    NextArg = (struct Value *)((char *)NextArg + MEM_ALIGN(sizeof(struct Value) + TypeStackSizeValue(NextArg)));
                    if (NextArg->Typ != FormatType && 
                            !((FormatType == &IntType || *FPos == 'f') && IS_NUMERIC_COERCIBLE(NextArg)) &&
                            !(FormatType == CharPtrType && (NextArg->Typ->Base == TypePointer || 
                                                             (NextArg->Typ->Base == TypeArray && NextArg->Typ->FromType->Base == TypeChar) ) ) )
					{
						l = StrPutStr(str,"XXX");str+=l;
					}
                    else
                    {
                        switch (*FPos)
                        {
                            case 's':
                            {
                                char *Str;
                                
                                if (NextArg->Typ->Base == TypePointer)
                                    Str = NextArg->Val->Pointer;
                                else
                                    Str = &NextArg->Val->ArrayMem[0];
                                    
                                if (Str == NULL)
								{
									l = StrPutStr(str,"NULL");str+=l;
								}
                                else
								{
									l = StrPutStr(str,Str);str+=l;
								}
                                break;
                            }
                            case 'd': l = sprintf(str,"%d",ExpressionCoerceInteger(NextArg));str+=l; break;
                            case 'u': l = sprintf(str,"%u",ExpressionCoerceUnsignedInteger(NextArg));str+=l; break;
                            case 'x': l = sprintf(str,"%x",ExpressionCoerceUnsignedInteger(NextArg));str+=l; break;
                            case 'b': l = sprintf(str,"%b",ExpressionCoerceUnsignedInteger(NextArg));str+=l; break;
                            case 'c': l = sprintf(str,"%c",ExpressionCoerceUnsignedInteger(NextArg));str+=l; break;
                            case 'f': l = sprintf(str,"%f",ExpressionCoerceFP(NextArg));str+=l; break;
                        }
                    }
                }
                
                ArgCount++;
            }
        }
        else
		{
            StrPutChar(str,*FPos);++str;
		}
    }
}

void StdioSprintf(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    char * buf;

    buf = Param[0]->Val->Pointer;

    GenericPrintf(Parser, ReturnValue, Param+1, NumArgs-1,buf);

    ReturnValue->Val->Integer = strlen(buf);
}

void StdioPuts(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    dConsolePut(Param[0]->Val->Pointer);
	dConsolePutChar ('\n');

    ReturnValue->Val->Integer = 0;
}

void StdioPrintf(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
	
    char buf[512];
    GenericPrintf(Parser, ReturnValue, Param, NumArgs,buf);
	dConsolePut (buf);
    ReturnValue->Val->Integer = strlen(buf);
	
}
void StdioCls(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
	dConsoleCls();
}
void StdioGets(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
	dConsoleGets(Param[0]->Val->Pointer);
	ReturnValue->Val->Pointer = Param[0]->Val->Pointer;
}

/* stdio calls */
void StdioFopen(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs) 
{
    ReturnValue->Val->Pointer = fopen(Param[0]->Val->Pointer, Param[1]->Val->Pointer);
}

void StdioFreopen(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs) 
{
    ReturnValue->Val->Pointer = freopen(Param[0]->Val->Pointer, Param[1]->Val->Pointer, Param[2]->Val->Pointer);
}

void StdioFclose(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs) 
{
    ReturnValue->Val->Integer = fclose(Param[0]->Val->Pointer);
}

void StdioFread(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs) 
{
    ReturnValue->Val->Integer = fread(Param[0]->Val->Pointer, Param[1]->Val->Integer, Param[2]->Val->Integer, Param[3]->Val->Pointer);
}

void StdioFwrite(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs) 
{
    ReturnValue->Val->Integer = fwrite(Param[0]->Val->Pointer, Param[1]->Val->Integer, Param[2]->Val->Integer, Param[3]->Val->Pointer);
}

void StdioFgetc(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs) 
{
    ReturnValue->Val->Integer = fgetc(Param[0]->Val->Pointer);
}

void StdioFgets(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs) 
{
    ReturnValue->Val->Pointer = fgets(Param[0]->Val->Pointer, Param[1]->Val->Integer, Param[2]->Val->Pointer);
}

void StdioFeof(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs) 
{
    ReturnValue->Val->Integer = feof((FILE *)Param[0]->Val->Pointer);
}

void StdioFputc(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs) 
{
    ReturnValue->Val->Integer = fputc(Param[0]->Val->Integer, Param[1]->Val->Pointer);
}

void StdioFputs(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs) 
{
    ReturnValue->Val->Integer = fputs(Param[0]->Val->Pointer, Param[1]->Val->Pointer);
}

void StdioFtell(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs) 
{
    ReturnValue->Val->Integer = ftell(Param[0]->Val->Pointer);
}

void StdioFseek(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs) 
{
    ReturnValue->Val->Integer = fseek(Param[0]->Val->Pointer, Param[1]->Val->Integer, Param[2]->Val->Integer);
}

void StdioSetPixel (struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs) 
{
	setPixel(Param[0]->Val->Integer,Param[1]->Val->Integer,Param[2]->Val->Integer);
}
void StdioClrscr (struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs) 
{
	clrscr();
}

const char StdioDefs[] = "\
typedef struct __va_listStruct va_list; \
typedef struct __FILEStruct FILE;\
";
/* all stdio functions */
struct LibraryFunction StdioFunctions[] =
{
    { StdioSprintf		,"int sprintf(char *, char *,...);" },
	{ StdioPuts			,"int puts(char *);"},
	{ StdioGets			,"char * gets(char *);"},
	{ StdioCls			,"void cls();"},
	
    { StdioFopen,   	"FILE *fopen(char *, char *);" },
    { StdioFreopen, 	"FILE *freopen(char *, char *, FILE *);" },
    { StdioFclose,  	"int fclose(FILE *);" },
    { StdioFread,  	 	"int fread(void *, int, int, FILE *);" },
    { StdioFwrite,  	"int fwrite(void *, int, int, FILE *);" },
    { StdioFgetc,   	"int fgetc(FILE *);" },
    { StdioFgetc,   	"int getc(FILE *);" },
    { StdioFgets,   	"char *fgets(char *, int, FILE *);" },
    { StdioFputc,   	"int fputc(int, FILE *);" },
    { StdioFputs,   	"int fputs(char *, FILE *);" },
    { StdioFeof,    	"int feof(FILE *);" },
    { StdioFtell,   	"int ftell(FILE *);" },
    { StdioFseek,   	"int fseek(FILE *, int, int);" },
	{ StdioPrintf,   	"int printf(char*,...);" },
    { NULL			,	""}
};

/* creates various system-dependent definitions */
void StdioSetupFunc(void)
{
	struct LibraryFunction *plib;
	plib = &StdioFunctions;
	
    plib[ 0].Func = StdioSprintf	;
	plib[ 1].Func = StdioPuts		;
	plib[ 2].Func = StdioGets		;
	plib[ 3].Func = StdioCls		;
	
	plib[ 4].Func = StdioFopen		;
	plib[ 5].Func = StdioFreopen	;
	plib[ 6].Func = StdioFclose		;
	plib[ 7].Func = StdioFread		;
	plib[ 8].Func = StdioFwrite		;
	plib[ 9].Func = StdioFgetc		;
	plib[10].Func = StdioFgetc		;
	plib[11].Func = StdioFgets		;
	plib[12].Func = StdioFputc		;
	plib[13].Func = StdioFputs		;
	plib[14].Func = StdioFeof		;
	plib[15].Func = StdioFtell		;
	plib[16].Func = StdioFseek		;
	
	plib[17].Func = StdioPrintf		;
	
    struct ValueType *StructFileType;
    struct ValueType *FilePtrType;

    /* make a "struct __FILEStruct" which is the same size as a native FILE structure */
    StructFileType = TypeCreateOpaqueStruct(NULL, TableStrRegister("__FILEStruct"), sizeof(FILE));
    
    /* get a FILE * type */
    FilePtrType = TypeGetMatching(NULL, StructFileType, TypePointer, 0, StrEmpty, TRUE);

    /* make a "struct __va_listStruct" which is the same size as our struct StdVararg */
    TypeCreateOpaqueStruct(NULL, TableStrRegister("__va_listStruct"), sizeof(FILE));
    
    /* define EOF equal to the system EOF */
    VariableDefinePlatformVar(NULL, "EOF", &IntType, (union AnyValue *)&EOFValue, FALSE);
    VariableDefinePlatformVar(NULL, "SEEK_SET", &IntType, (union AnyValue *)&SEEK_SETValue, FALSE);
    VariableDefinePlatformVar(NULL, "SEEK_CUR", &IntType, (union AnyValue *)&SEEK_CURValue, FALSE);
    VariableDefinePlatformVar(NULL, "SEEK_END", &IntType, (union AnyValue *)&SEEK_ENDValue, FALSE);
    VariableDefinePlatformVar(NULL, "BUFSIZ", &IntType, (union AnyValue *)&BUFSIZValue, FALSE);
    VariableDefinePlatformVar(NULL, "FILENAME_MAX", &IntType, (union AnyValue *)&FILENAME_MAXValue, FALSE);

    /* define NULL*/
    if (!VariableDefined(TableStrRegister("NULL")))
        VariableDefinePlatformVar(NULL, "NULL", &IntType, (union AnyValue *)&ZeroValue, FALSE);
}
