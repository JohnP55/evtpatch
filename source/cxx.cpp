#include <spm/memory.h>
#include <spm/system.h>

using spm::memory::Heap;

void * operator new(size_t size)
{
    return spm::memory::__memAlloc(Heap::HEAP_MAIN, size);
}

void * operator new[](size_t size)
{
    return spm::memory::__memAlloc(Heap::HEAP_MAIN, size);
}

void operator delete(void * ptr)
{
    return spm::memory::__dl__FPv(ptr);
}

void operator delete[](void * ptr)
{
    return spm::memory::__dl__FPv(ptr);
}

void operator delete(void * ptr, size_t size)
{
    (void) size;
    return spm::memory::__dl__FPv(ptr);
}

void operator delete[](void * ptr, size_t size)
{
    (void) size;
    return spm::memory::__dl__FPv(ptr);
}

// With custom heap parameter
void * operator new(size_t size, Heap heap)
{
    return spm::memory::__memAlloc(heap, size);
}

void * operator new[](size_t size, Heap heap)
{
    return spm::memory::__memAlloc(heap, size);
}

// Without heap parameter
void * malloc(size_t size) {
    return (void*)new u8[size];
}
