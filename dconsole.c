#include <nspireio2.h>

static nio_console csl;

void dConsoleInit ()
{
	// 53 columns, 29 rows. 0px offset for x/y. Background color 0 (black), foreground color 15 (white)
	nio_InitConsole(&csl, 53, 29, 0, 0, 15,0);
}
void dConsoleCleanUp()
{
	nio_CleanUp(&csl);
}

void dConsolePut(const char * str)
{
	nio_PrintStr(&csl,str);
}
void dConsolePutChar(char c)
{
	 nio_PrintChar(&csl,c);
}
int	dPrintf	(const char *format,...)
{
	char	buf[512];
	int		length;
	va_list	arg_list;

	va_start(arg_list,format);
	length = vsprintf(buf,format,arg_list);
	va_end(arg_list);

	dConsolePut (buf);

	return length;
}

void dConsoleCls()
{
	nio_Clear(&csl);
}
void dConsoleRefresh()
{
	nio_DrawConsole(&csl);
}
void dConsoleGets(char * s)
{
	nio_GetStr(&csl,s);
}