#include <stdlib.h>
#include <malloc.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#define WSIZE 4
#define DSIZE 8
#define CHUNKSIZE (1 << 12)

#define MAX(X,Y) ((X) > (Y) ? (X) : (Y))
#define PACK(size,alloc) ((size) | (alloc))

#define GET(p) (*(unsigned int*)(p))
#define PUT(p,val) (*(unsigned int*)(p) = (val))

#define GET_SIZE(p)(GET(p) & ~0X7)
#define GET_ALLOC(p) (GET(p) & 0x1)
#define GET_PRE_ALLOC(p) (GET(p) & 0x2)

//获取数据块的头部和尾部指针
#define HDRP(bp) ((char*)(bp) - WSIZE)
#define FTRP(bp) ((char*)(bp) + GET_SIZE(HDRP(bp)) - DSIZE)

//获取下一个和上一个数据块
#define NEXT_BLKP(bp) ((char*)(bp) + GET_SIZE((char*)(bp) - WSIZE))
#define PREV_BLKP(bp) ((char*)(bp) - GET_SIZE((char*)(bp) - DSIZE))

static char* heap_listp = 0;
//指向下一个合适的数据块
static char* rover;
static char* mem_heap;
static char* mem_brk;
static char* mem_max_addr;

#define MAX_HEAP (10*1024)
//初始化分配空间
void mem_init (void) {
    mem_heap = (char*) malloc(MAX_HEAP);
    assert(mem_heap != NULL);
    mem_brk = (char*)mem_heap;
    mem_max_addr = (char* )(mem_heap + MAX_HEAP);
}
//扩展堆
void* mem_sbrk (int num) {
    char* old_brk = mem_brk;
    assert(num > 0 && ((mem_brk + num) < mem_max_addr));
    mem_brk += num;
    return (void *)old_brk;
}

//扩展堆
static void *extern_heap(size_t words);
//存放值
static void place(void *bp, size_t asize);
//查找合适的数据块
static void *find_fit(size_t asize);
//合并数据块
static void *coalesce(void *bp);

//创建空闲链表并进行初始化
int mm_init (void) {
    if((heap_listp = mem_sbrk(4 * WSIZE)) == (void *)-1)
        return -1;
    PUT(heap_listp , 0);
    PUT(heap_listp + (1*WSIZE) ,PACK(DSIZE , 0x1 | 0x2));
    PUT(heap_listp + (2*WSIZE) ,PACK(DSIZE , 0x1 | 0x2));
    PUT(heap_listp + (3*WSIZE) ,PACK(0 , 0x1 | 0x2));

    heap_listp += (2*WSIZE);
    rover = heap_listp;

    if(extern_heap(CHUNKSIZE/WSIZE) == NULL)
        return -1;
    return 0;
}
static void *extern_heap(size_t words) {
    char* bp;
    size_t size;
    size = (words % 2) ? WSIZE *(words + 1) : WSIZE * words;
    if((long)(bp = mem_sbrk(size)) == -1) return NULL;
    if(GET_PRE_ALLOC(HDRP(bp))) PUT(HDRP(bp),PACK(size,0x2));
    else PUT(HDRP(bp),PACK(size,0));
    PUT(FTRP(bp),PACK(size,0));
    PUT(HDRP(NEXT_BLKP(bp)),PACK(0,1));
    return coalesce(bp);
}

//释放和合并块
void mm_free (void *bp) {
    if(bp == 0) return;
    size_t size = GET_SIZE(HDRP(bp));

    if(heap_listp == 0) mem_init();
    if(GET_PRE_ALLOC(bp)) PUT(HDRP(bp),PACK(size,0x2));
    else PUT(HDRP(bp),PACK(size,0));
    PUT(FTRP(bp),PACK(size,0));
    //将对应位清零
    *HDRP(NEXT_BLKP(bp)) &= ~0x2;
    coalesce(bp);
}
static void *coalesce(void *bp) {
    size_t prev_alloc = GET_PRE_ALLOC(HDRP(bp));
    size_t next_alloc = GET_ALLOC(HDRP(NEXT_BLKP(bp)));
    size_t size = GET_SIZE(HDRP(bp));

    if (prev_alloc && next_alloc) return bp;
    else if (prev_alloc && !next_alloc) {
        size += GET_SIZE(HDRP(NEXT_BLKP(bp)));
        PUT(HDRP(bp),PACK(size,0x2));
        PUT(FTRP(bp),PACK(size,0));
    } else if(!prev_alloc && next_alloc) {
        size += GET_SIZE(HDRP(PREV_BLKP(bp)));
        if(GET_PRE_ALLOC(PREV_BLKP(bp))) PUT(HDRP(PREV_BLKP(bp)),PACK(size,0x2));
        else PUT(HDRP(PREV_BLKP(bp)),PACK(size,0));
        PUT(FTRP(bp),PACK(size,0));
        bp = PREV_BLKP(bp);  
    } else if(!prev_alloc && !next_alloc) {
        size = GET_SIZE(HDRP(PREV_BLKP(bp))) + GET_SIZE(FTRP(NEXT_BLKP(bp)));
        if(GET_PRE_ALLOC(PREV_BLKP(bp))) PUT(HDRP(PREV_BLKP(bp)),PACK(size,0x2));
        else PUT(HDRP(PREV_BLKP(bp)),PACK(size,0));
        PUT(FTRP(NEXT_BLKP(bp)),PACK(size,0));
        bp = PREV_BLKP(bp);
    }
    //当rover指针没有指向任何空闲的数据块时，仅合并
    if((rover > (char*)bp) && (rover < NEXT_BLKP(bp)))
        rover = bp;
    return bp;
}

//分配算法== >>下一次适配
void *find_fit(size_t asize) {
    char *oldrover = rover;
    while (GET_SIZE(HDRP(rover)) > 0) {
        if(!GET_ALLOC(HDRP(rover)) && asize < GET_SIZE(HDRP(rover))) 
            return rover;
        rover = NEXT_BLKP(rover);
    }
    for(rover = heap_listp; rover < oldrover; rover = NEXT_BLKP(rover)) {
        if(!GET_ALLOC(HDRP(rover)) && (asize <= GET_SIZE(HDRP(rover))))
            return rover;
    }
    return NULL;
}

//分割数据块
void place(void *bp, size_t asize) {
    size_t csize = GET_SIZE(HDRP(bp));
    if((csize - asize) >= DSIZE) {
        if(GET_PRE_ALLOC(HDRP(bp))) PUT(HDRP(bp),PACK(asize,0x1 | 0x2));
        else PUT(HDRP(bp),PACK(asize,1));
        PUT(FTRP(bp),PACK(asize,1));
        bp = NEXT_BLKP(bp);
        PUT(HDRP(bp),PACK(csize-asize,0x2));
        PUT(FTRP(bp),PACK(csize-asize,0));
    }
    else {
        if(GET_PRE_ALLOC(HDRP(bp)))  PUT(HDRP(bp),PACK(csize,0x2|0x1));
        else PUT(HDRP(bp),PACK(csize,1));
        PUT(FTRP(bp),PACK(csize,1));
        *HDRP(NEXT_BLKP(bp)) |= 0x2;
    }
}
//分配块
void *mm_malloc(size_t size) {
    size_t asize;
    size_t exsize;
    char *bp;
    if(heap_listp == 0) mem_init();
    if(size == 0) return NULL;
    /*调整块的大小
      最小块的大小为12 Bytes
      对于超过8字节的请求，向上舍入最接近8的整数倍
    */
    if(size <= DSIZE) 
        asize = DSIZE + WSIZE;
    else 
        asize = DSIZE *((size + DSIZE + (WSIZE - 1)) / DSIZE);
    
    if((bp = find_fit(asize)) != NULL) {
        place(bp,asize);
        return bp;
    } 

    exsize = MAX(asize,CHUNKSIZE);
    if((bp = extern_heap(exsize/WSIZE)) == NULL) return NULL;
    place(bp,asize);
    return bp;

}

int main (void) {
    mem_init();
    mm_init();
    char * p = mm_malloc(6);
    strcpy(p,"linux");
    printf("string:%s\n",p);
    mm_free(p);
    printf("string After:%s\n",p);
    return 0;
}