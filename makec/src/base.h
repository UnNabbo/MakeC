#ifdef _WIN32

#include <windows.h>
#include "tlhelp32.h"

#endif

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#ifndef _MSC_VER
#error Makec currenlty only support msvc compilation
#endif

// only valid for 64 bit CPUs
typedef signed char        s8;
typedef signed short       s16;
typedef signed int         s32;
typedef signed long long   s64;
typedef unsigned char      u8;
typedef unsigned short     u16;
typedef unsigned int       u32;
typedef unsigned long long u64;
typedef float              f32;
typedef double             f64;
typedef unsigned char      b8;
typedef unsigned char      b16;
typedef unsigned int       b32;
typedef unsigned long long b64;

#ifndef __cplusplus
#define true 1
#define false 0
#endif

#undef ZeroMemory
#define ZeroMemory(var) memset(&var, 0, sizeof(var))
#define MemAlloc(size) calloc(1, size)
#define MemRealloc(ptr, size) realloc(ptr, size)
#define MemFree(ptr) free(ptr)
#define MemCopy(dest, src, size) memcpy(dest, src, size)
#define Assert(cond, text, ...) if(!(cond)){printf(text, __VA_ARGS__); exit(1);}

void * _MemReplicateInternal(void * Ptr, u64 Size){
	void * Alloc = MemAlloc(Size);
	MemCopy(Alloc, Ptr, Size);
	return Alloc;
}
#define MemReplicate(ptr) _MemReplicateInternal(ptr, sizeof(*ptr));

#define INLINE __forceinline 
