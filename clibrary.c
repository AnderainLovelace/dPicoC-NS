#include "picoc.h"
#include "interpreter.h"

/* the picoc version string */
static const char *VersionString = NULL;

/* endian-ness checking */
static const int __ENDIAN_CHECK__ = 1;

static int BigEndian = 0;
static int LittleEndian = 0;


/* global initialisation for libraries */
void LibraryInit()
{
    /* define the version number macro */
    VersionString = TableStrRegister(PICOC_VERSION);
    VariableDefinePlatformVar(NULL, "PICOC_VERSION", CharPtrType, (union AnyValue *)&VersionString, FALSE);

    /* define endian-ness macros */
    BigEndian = ((*(char*)&__ENDIAN_CHECK__) == 0);
    LittleEndian = ((*(char*)&__ENDIAN_CHECK__) == 1);

    VariableDefinePlatformVar(NULL, "BIG_ENDIAN", &IntType, (union AnyValue *)&BigEndian, FALSE);
    VariableDefinePlatformVar(NULL, "LITTLE_ENDIAN", &IntType, (union AnyValue *)&LittleEndian, FALSE);
}

/* add a library */

void LibraryAdd(struct Table *GlobalTable, const char *LibraryName, struct LibraryFunction *FuncList)
{
    struct ParseState Parser;
    int Count;
    char *Identifier;
    struct ValueType *ReturnType;
    struct Value *NewValue;
    void *Tokens;
    const char *IntrinsicName = TableStrRegister(LibraryName);
    static int max = 0;
    for (Count = 0; FuncList[Count].Prototype[0] != 0; Count++)
    {
		/*{
			dPrintf("DEBUG[%s][%s]\n",LibraryName,FuncList[Count].Prototype);
			wait_key_pressed();
		}*/
		/*{
			int l = strlen(FuncList[Count].Prototype);
			if (l>max)
			{
				printf("new max:%d\n",max=l);
			}
		}*/
        Tokens = LexAnalyse(IntrinsicName, FuncList[Count].Prototype, strlen((char *)FuncList[Count].Prototype), NULL);
        LexInitParser(&Parser, FuncList[Count].Prototype, Tokens, IntrinsicName, TRUE);
        TypeParse(&Parser, &ReturnType, &Identifier, NULL);
        NewValue = ParseFunctionDefinition(&Parser, ReturnType, Identifier);
        NewValue->Val->FuncDef.Intrinsic = FuncList[Count].Func;
        HeapFreeMem(Tokens);
    }
}

/* portability-related I/O calls */
void PrintCh(char OutCh)
{
    dConsolePutChar(OutCh);
}

void PrintSimpleInt(long Num)
{
    dPrintf("%ld", Num);
}

void PrintStr(const char *Str)
{
    dConsolePut(Str);
}

void PrintFP(double Num)
{
    dPrintf("%f", Num);
}
void PrintType(struct ValueType *Typ)
{
    switch (Typ->Base)
    {
        case TypeVoid:          PrintStr("void"); break;
        case TypeInt:           PrintStr("int"); break;
        case TypeShort:         PrintStr("short"); break;
        case TypeChar:          PrintStr("char"); break;
        case TypeLong:          PrintStr("long"); break;
        case TypeUnsignedInt:   PrintStr("unsigned int"); break;
        case TypeUnsignedShort: PrintStr("unsigned short"); break;
        case TypeUnsignedLong:  PrintStr("unsigned long"); break;
        case TypeFP:            PrintStr("double"); break;
        case TypeFunction:      PrintStr("function"); break;
        case TypeMacro:         PrintStr("macro"); break;
        case TypePointer:       if (Typ->FromType) PrintType(Typ->FromType); PrintCh('*'); break;
        case TypeArray:         PrintType(Typ->FromType); PrintCh('['); if (Typ->ArraySize != 0) PrintSimpleInt(Typ->ArraySize); PrintCh(']'); break;
        case TypeStruct:        PrintStr("struct "); PrintStr(Typ->Identifier); break;
        case TypeUnion:         PrintStr("union "); PrintStr(Typ->Identifier); break;
        case TypeEnum:          PrintStr("enum "); PrintStr(Typ->Identifier); break;
        case TypeGotoLabel:     PrintStr("goto label "); break;
        case Type_Type:         PrintStr("type "); break;
    }
}

