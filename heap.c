/* stack grows up from the bottom and heap grows down from the top of heap space */
#include "interpreter.h"

#define FREELIST_BUCKETS 8                          /* freelists for 4, 8, 12 ... 32 byte allocs */
#define SPLIT_MEM_THRESHOLD 16                      /* don't split memory which is close in size */

static unsigned char *HeapMemory = NULL;            /* stack memory since our heap is malloc()ed */
static void *HeapBottom = NULL;                     /* the bottom of the (downward-growing) heap */
static void *StackFrame = NULL;                     /* the current stack frame */
void *HeapStackTop = NULL;                          /* the top of the stack */

static struct AllocNode *FreeListBucket[FREELIST_BUCKETS];      /* we keep a pool of freelist buckets to reduce fragmentation */
static struct AllocNode *FreeListBig;                           /* free memory which doesn't fit in a bucket */


/* initialise the stack and heap storage */
void HeapInit(int StackOrHeapSize)
{
    int Count;
    int AlignOffset = 0;
    
    HeapMemory = malloc(StackOrHeapSize);

    while (((unsigned long)&HeapMemory[AlignOffset] & (sizeof(ALIGN_TYPE)-1)) != 0)
        AlignOffset++;
        
    StackFrame = &HeapMemory[AlignOffset];
    HeapStackTop = &HeapMemory[AlignOffset];
    *(void **)StackFrame = NULL;
    HeapBottom = &HeapMemory[StackOrHeapSize-sizeof(ALIGN_TYPE)+AlignOffset];
    FreeListBig = NULL;
    for (Count = 0; Count < FREELIST_BUCKETS; Count++)
        FreeListBucket[Count] = NULL;
}

void HeapCleanup()
{
    free(HeapMemory);
}

/* allocate some space on the stack, in the current stack frame
 * clears memory. can return NULL if out of stack space */
void *HeapAllocStack(int Size)
{
    char *NewMem = HeapStackTop;
    char *NewTop = (char *)HeapStackTop + MEM_ALIGN(Size);
	//printf("HeapAllocStack(%ld) at 0x%lx\n", (unsigned long)MEM_ALIGN(Size), (unsigned long)HeapStackTop);
	//WaitKey();
    if (NewTop > (char *)HeapBottom)
        return NULL;
	//printf("sss");
	//WaitKey();
    HeapStackTop = (void *)NewTop;
    memset((void *)NewMem, '\0', Size);
    return NewMem;
}

/* allocate some space on the stack, in the current stack frame */
void HeapUnpopStack(int Size)
{
    HeapStackTop = (void *)((char *)HeapStackTop + MEM_ALIGN(Size));
}

/* free some space at the top of the stack */
int HeapPopStack(void *Addr, int Size)
{
    int ToLose = MEM_ALIGN(Size);
    if (ToLose > ((char *)HeapStackTop - (char *)&HeapMemory[0]))
        return FALSE;
    
    HeapStackTop = (void *)((char *)HeapStackTop - ToLose);
    assert(Addr == NULL || HeapStackTop == Addr);
    
    return TRUE;
}

/* push a new stack frame on to the stack */
void HeapPushStackFrame()
{
    *(void **)HeapStackTop = StackFrame;
    StackFrame = HeapStackTop;
    HeapStackTop = (void *)((char *)HeapStackTop + MEM_ALIGN(sizeof(ALIGN_TYPE)));
}

/* pop the current stack frame, freeing all memory in the frame. can return NULL */
int HeapPopStackFrame()
{
    if (*(void **)StackFrame != NULL)
    {
        HeapStackTop = StackFrame;
        StackFrame = *(void **)StackFrame;
        return TRUE;
    }
    else
        return FALSE;
}

/* allocate some dynamically allocated memory. memory is cleared. can return NULL if out of memory */
void *HeapAllocMem(int Size)
{
    return calloc(Size, 1);
}

/* free some dynamically allocated memory */
void HeapFreeMem(void *Mem)
{
    free(Mem);
}

