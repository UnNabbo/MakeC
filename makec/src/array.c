#define ARRAY_DEF_SIZE 16
typedef struct{
    s32 Size;
    s32 Reserved;
    u64 Stride;
} _internal_array_layout;

#define _ArrayInternal(Array) ((_internal_array_layout*)(((s8*)Array) - sizeof(_internal_array_layout)))

#define _Temporary(x, Name) __typeof__((x)) Name = (x)

static void * _ArrayAllocInternal(u64 Stride){
    _internal_array_layout* Array = MemAlloc(sizeof(_internal_array_layout) + Stride * ARRAY_DEF_SIZE);

    Array->Size = 0;
    Array->Reserved = ARRAY_DEF_SIZE;
    Array->Stride = Stride;
    
    s8* Data = ((s8*)Array) + sizeof(_internal_array_layout);
    return Data;
}

static void _ArrayReallocInternal(u8** Base, u64 NewSize){
    _internal_array_layout* Array = _ArrayInternal(*Base);
    Array->Reserved = NewSize;
    Array = MemRealloc(Array, sizeof(_internal_array_layout) + Array->Stride * Array->Reserved);
	*Base = ((s8*)Array) + sizeof(_internal_array_layout);
}

static void * _ArrayAppendInternal(u8 ** Base, void * Data){
	Assert(*Base, "[ERROR] Array was not inizialized");
    _internal_array_layout* Array = _ArrayInternal(*Base);
    if(Array->Size + 1 > Array->Reserved){
        _ArrayReallocInternal(Base, Array->Reserved * 8);
		Array = _ArrayInternal(*Base);
    }
    u8* Slot = *Base + Array->Size++ * Array->Stride;
    MemCopy(Slot, Data, Array->Stride);
	return Slot;
}

static void * _ArrayCopyInternal(u8 ** Base){
    _internal_array_layout* ToCopy = _ArrayInternal(*Base);
	_internal_array_layout* Array = MemAlloc(sizeof(_internal_array_layout) + ToCopy->Stride * ToCopy->Reserved);

    MemCopy(Array, ToCopy, sizeof(_internal_array_layout) + ToCopy->Stride * ToCopy->Reserved);
    return ((s8*)Array) + sizeof(_internal_array_layout);
}

#define ArrayAlloc(Type) _ArrayAllocInternal(sizeof(Type))
#define ArrayCopy(Array) _ArrayCopyInternal((u8**)&Array)
#define ArrayFree(Array) MemFree((void*)_ArrayInternal(Array))
#define ArrayFreeAll(Array) for_each(item, Array){MemFree(item);};MemFree((void*)_ArrayInternal(Array))
#define ArraySize(Array) _ArrayInternal(Array)->Size
#define ArrayReserved(Array) _ArrayInternal(Array)->Reserved

#define ArrayAppend(Array, Data) _ArrayAppendInternal((u8**)Array, Data)

#define for_each(Var, Array)\
for(int Var##Index = 0, _=1;_;_=0) \
for(__typeof__((Array[0])) Var = (Array[0]); Var##Index < ArraySize(Array); Var = Array[++Var##Index])
