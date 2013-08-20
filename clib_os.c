#include <nspireio2.h>
#include "interpreter.h"
extern int lcd_type;

extern void write_graph(int x,int y,int width,int height,unsigned char * pimage,int cl_fg,int cl_bg);
extern void dg_put_str (int x,int y,char * s,int cl_fg,int cl_bg);

inline int getPixel(int x, int y) {
  unsigned char* p = (unsigned char*)(SCREEN_BASE_ADDRESS + ((x >> 1) + (y << 7) + (y << 5)));
  return ((x & 1) ? (*p & 0x0F) : (*p >> 4));
}
inline void set_pixel_buf(void* scrbuf, unsigned x, unsigned y, uint16_t color){
    if(x < SCREEN_WIDTH && y < SCREEN_HEIGHT){
        if(!lcd_type){
            uint8_t* p = (uint8_t*) scrbuf + (x >> 1) + (y << 7) + (y << 5);
            if(x%2)
                *p = (*p & 0xF0) | color;
            else
                *p = (*p & 0x0F) | color << 4;
            //*p = (x % 2) ? ((*p & 0b11110000) | color) : ((*p & 0b00001111) | (color << 4));
            //*p = (*p & (0b1111 << (x%2)*4)) | (color << (1 - (x%2))*4);
        }
        else{
            uint16_t* p = (uint16_t*) (scrbuf + (x << 1) + (y << 9) + (y << 7));//(x << 1) + (y << 9) + (y << 7);
            *p = color;
        }
    }
}

inline int get_pixel_buf(unsigned char * scrbuf,int x, int y) {
  unsigned char* p = (unsigned char*)(scrbuf + ((x >> 1) + (y << 7) + (y << 5)));
  return ((x & 1) ? (*p & 0x0F) : (*p >> 4));
}

void write_graph_buf(unsigned char * scrbuf,int x,int y,int width,int height,unsigned char * pimage,int cl_fg,int cl_bg)
{
	int i,j,k,pixel,rx=0,ry=0;
	unsigned char p;
    int iwidth = width/8 + (width % 8 ? 1:0);
	for (i=0;i<height;++i,pimage+=iwidth)
	{
		ry = y+i;
		if (ry>=240) return;
		else if (ry<0) continue;
		for (j=0;j<iwidth;++j)
		{
			p = pimage[j];
			for (k=0;k<8;++k)
			{
				rx = x+(8-k)+8*j;
				pixel = p % 2;
				p>>=1;
				if (pixel)	set_pixel_buf(scrbuf,rx,ry,cl_fg);
				else		set_pixel_buf(scrbuf,rx,ry,cl_bg);
			}
		}
	}
}


void OsSetPixel(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
	setPixel(Param[0]->Val->Integer,Param[1]->Val->Integer,Param[2]->Val->Integer);
}
void OsGetPixel(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    ReturnValue->Val->Integer = getPixel(Param[0]->Val->Integer,Param[1]->Val->Integer);
}
void OsColorMode(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    ReturnValue->Val->Integer = lcd_isincolor;
}
void OsWriteGraph(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
	write_graph (	Param[0]->Val->Integer,Param[1]->Val->Integer,
					Param[2]->Val->Integer,Param[3]->Val->Integer,
					Param[4]->Val->Pointer,
					Param[5]->Val->Integer,Param[6]->Val->Integer);
}
void OsWaitKey(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
	wait_key_pressed();
}
void OsWaitNoKey(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
	wait_no_key_pressed();
}
void OsGetch(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
	ReturnValue->Val->Integer = nio_getch();
}
void OsClrScr(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
	clrscr();
}
void OsGetScrBaseAddr(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
	ReturnValue->Val->Pointer = SCREEN_BASE_ADDRESS;
}
void OsGetScrByteSize(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
	ReturnValue->Val->Pointer = SCREEN_BYTES_SIZE;
}
void OsMsgBox(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
	ReturnValue->Val->Integer = show_msgbox(Param[0]->Val->Pointer,Param[1]->Val->Pointer);
}
void OsMsgBox2B(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
	ReturnValue->Val->Integer = show_msgbox_2b(Param[0]->Val->Pointer,Param[1]->Val->Pointer,Param[2]->Val->Pointer,Param[3]->Val->Pointer);
}
void OsMsgBox3B(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
	ReturnValue->Val->Integer = show_msgbox_3b(Param[0]->Val->Pointer,Param[1]->Val->Pointer,Param[2]->Val->Pointer,Param[3]->Val->Pointer,Param[4]->Val->Pointer);
}
void OsSetPixelBuf(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
	set_pixel_buf(Param[0]->Val->Pointer,Param[1]->Val->Integer,Param[2]->Val->Integer,Param[3]->Val->Integer);
}
void OsGetPixelBuf(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
	get_pixel_buf(Param[0]->Val->Pointer,Param[1]->Val->Integer,Param[2]->Val->Integer);
}
void OsSleep(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
	sleep(Param[0]->Val->Integer);
}
void OsUsrInput(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
	ReturnValue->Val->Integer = show_msg_user_input(Param[0]->Val->Pointer,Param[1]->Val->Pointer,Param[2]->Val->Pointer,Param[3]->Val->Pointer);
}
void OsNumInput1(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
	char * title,*subtitle,*msg;
	int	 * value_ref,min,max;
	title		= Param[0]->Val->Pointer;
	subtitle	= Param[1]->Val->Pointer;
	msg			= Param[2]->Val->Pointer;
	value_ref	= Param[3]->Val->Pointer;
	min			= Param[4]->Val->Integer;
	max			= Param[5]->Val->Integer;
	ReturnValue->Val->Integer = show_1numeric_input(title,subtitle,msg,value_ref,min,max);
}
void OsNumInput2(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
	char * title,*subtitle,*msg1,*msg2;
	int	 * value_ref1,min1,max1,* value_ref2,min2,max2;
	title		= Param[0]->Val->Pointer;
	subtitle	= Param[1]->Val->Pointer;
	
	msg1		= Param[2]->Val->Pointer;
	value_ref1	= Param[3]->Val->Pointer;
	min1		= Param[4]->Val->Integer;
	max1		= Param[5]->Val->Integer;

	msg2		= Param[6]->Val->Pointer;
	value_ref2	= Param[7]->Val->Pointer;
	min2		= Param[8]->Val->Integer;
	max2		= Param[9]->Val->Integer;
	
	ReturnValue->Val->Integer = show_2numeric_input(title,subtitle,msg1,value_ref1,min1,max1,msg2,value_ref2,min2,max2);
}
void OsPutStr(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
	int x,y,clf,clb;char * str;
	x	= Param[0]->Val->Integer;
	y	= Param[1]->Val->Integer;
	str	= Param[2]->Val->Pointer;
	clf	= Param[3]->Val->Integer;
	clb	= Param[4]->Val->Integer;
	dg_put_str(x,y,str,clf,clb);
}
void OsWriteGraphBuf(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
	int x,y,clf,width,height,clb;unsigned char * scrbuf,*p;
	scrbuf	= Param[0]->Val->Pointer;
	x		= Param[1]->Val->Integer;
	y		= Param[2]->Val->Integer;
	width	= Param[3]->Val->Integer;
	height	= Param[4]->Val->Integer;
	p		= Param[5]->Val->Pointer;
	clf		= Param[6]->Val->Integer;
	clb		= Param[7]->Val->Integer;
	write_graph_buf(scrbuf,x,y,width,height,p,clf,clb);
}
struct LibraryFunction OsFunctions[] =
{
	{OsColorMode			,"int  is_incolor();"},
    {OsSetPixel				,"void setpixel(int,int,int);"},
	{OsGetPixel				,"int  getpixel(int,int);"},
    {OsSetPixelBuf			,"void setpixelbuf(unsigned char*,int,int,int);"},
	{OsGetPixelBuf			,"int  getpixelbuf(unsigned char*,int,int);"},
	{OsWaitKey				,"void wait_key();"},
	{OsWaitNoKey			,"void wait_no_key();"},
	{OsGetch				,"int  getch();"},
	{OsClrScr				,"void clrscr();"},
	{OsGetScrBaseAddr		,"unsigned char * get_scr_base_addr();"},
	{OsGetScrByteSize		,"int  get_scr_byte_size();"},
	{OsMsgBox				,"int  msgbox  (char*,char*);"},
	{OsMsgBox2B				,"int  msgbox2b(char*,char*,char*,char*);"},
	{OsMsgBox3B				,"int  msgbox3b(char*,char*,char*,char*,char*);"},
	{OsSleep				,"void sleep(int);"},
	{OsUsrInput				,"int  usr_input(char*,char*,char*,char**);"},
	{OsNumInput1			,"int  num_input1(char*,char*,char*,int*,int,int);"},
	{OsNumInput2			,"int  num_input2(char*,char*,char*,int*,int,int,char*,int*,int,int);"},
	{OsPutStr				,"void putstr(int,int,char*,int,int);"},
	{OsWriteGraph			,"void write_graph(int,int,int,int,unsigned char*,int,int);"},
	{OsWriteGraphBuf		,"void write_graph_buf(unsigned char*,int,int,int,int,unsigned char*,int,int);"},
	{NULL,""}
};

void OsSetupFunc(void)
{
	struct LibraryFunction *plib;
	plib = OsFunctions;
	plib[ 0].Func = OsColorMode			;
    plib[ 1].Func = OsSetPixel			;
	plib[ 2].Func = OsGetPixel			;
    plib[ 3].Func = OsSetPixelBuf		;
	plib[ 4].Func = OsGetPixelBuf		;
	plib[ 5].Func = OsWaitKey			;
	plib[ 6].Func = OsWaitNoKey			;
	plib[ 7].Func = OsGetch				;
	plib[ 8].Func = OsClrScr			;
	plib[ 9].Func = OsGetScrBaseAddr	;
	plib[10].Func = OsGetScrByteSize	;
	plib[11].Func = OsMsgBox			;
	plib[12].Func = OsMsgBox2B			;
	plib[13].Func = OsMsgBox3B			;
	plib[14].Func = OsSleep				;
	plib[15].Func = OsUsrInput			;
	plib[16].Func = OsNumInput1			;
	plib[17].Func = OsNumInput2			;
	plib[18].Func = OsPutStr			;
	plib[19].Func =	OsWriteGraph		;
	plib[20].Func = OsWriteGraphBuf		;
}
