#ifndef DCONSOLE_H
#define DCONSOLE_H

void		dConsolePut				(const char *);
void		dConsolePutChar 		(char);
int			dPrintf					(const char *,...);
void		dConsoleInit 			();
void		dConsoleCleanUp			();
void		dConsoleRefresh			();
void		dConsoleCls				();
void 		dConsoleGets			(char *);
#endif