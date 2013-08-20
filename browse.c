#include <os.h>
extern int lcd_type;
#define COLOR_BLACK		(0)
#define COLOR_WHITE		(lcd_type ? 0xFFFF : 0xF)

extern void dg_put_str (int x,int y,char * s,int cl_fg,int cl_bg);

int dirlist(const char* path,const char* pattern, char** result,const char * filter)
{
	unsigned char StructDstat[512];
	chdir(path);
	if (NU_Get_First((struct dstat*)StructDstat, pattern))
	{
		NU_Done((struct dstat*)StructDstat);
		return -1;
	}
	int i = 0;
	do
	{
		char* dirname,*fname;
		fname = (char*) &StructDstat[13];
		if (strstr(fname,filter))
		{
			dirname = (char*) malloc(128);
			strcpy(dirname, (char*) fname);
			result[i] = dirname;
			i++;
		}
	} while (NU_Get_Next((struct dstat*)StructDstat) == 0); 
	NU_Done((struct dstat*)StructDstat);
	return i;
}

BOOL file_browse (const char * path,const char * filter,const char *title,char * filename)
{
	int			numfiles,i,top,bottom,index = 0;
	const int	h_max = 19;
	char *		filenames[512];
	BOOL		refresh = TRUE,retvalue = FALSE;
	const int	font_height = 12;
	int			title_x;
	const int	left = 0;
	// get file names
	numfiles = dirlist(path,"*.*",filenames,filter);

	if (numfiles<=0)
	{
		clrscr();
		dg_put_str(0,0,"No such files exist!",COLOR_WHITE,COLOR_BLACK);
		wait_key_pressed();
		return FALSE;
	}
	//
	title_x = (320-strlen(title)*8)/2;
	top = 0;bottom = h_max;
	while(TRUE)
	{
		if (refresh)
		{
			refresh = FALSE;
			int y;

			clrscr();
			dg_put_str(title_x,0,title,15,0);
			
			if (bottom-top>h_max)bottom = top + h_max;
			if(top>index)
			{
				top = index;
				bottom = index + h_max;
			}
			if (index>bottom)
			{
				bottom = index;
				top = bottom - h_max;
			}
			if(bottom>numfiles - 1)bottom = numfiles - 1;
			
			for (i=top;i<=bottom;++i)
			{
				y = i-top+1;
				if (index!=i)
					dg_put_str(left,y*font_height,filenames[i],COLOR_WHITE,COLOR_BLACK);
				else
					dg_put_str(left,y*font_height,filenames[i],COLOR_BLACK,COLOR_WHITE);
			}
		}
		wait_key_pressed();
		if (isKeyPressed(KEY_NSPIRE_UP)) 
			{if(--index<0) index = numfiles - 1;refresh = TRUE;}
		else if (isKeyPressed(KEY_NSPIRE_DOWN)) 
			{if(++index>numfiles - 1) index = 0;refresh = TRUE;}
		else if (isKeyPressed(KEY_NSPIRE_ESC))
		{
			* filename = 0;
			retvalue = FALSE;
			break;
		}
		else if (isKeyPressed(KEY_NSPIRE_ENTER))
		{
			retvalue = TRUE;
			strcpy(filename,filenames[index]);
			break;
		}
	}
	// clean up
	for(i=0;i<numfiles;i++)
	{
		free(filenames[i]);
	}
	return retvalue;
}

