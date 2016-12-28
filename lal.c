// (c) 2016 ZaKlaus; All Rights Reserved

#if !defined(HFTW_MEM_H)

doc_cat(Memory Arena)

doc(arena_header)
doc_string(Describes arena elements.)

typedef struct
{
    s32 Size;    // Size of the element.
    u32 Tag;     // Element's tag used for identification.
    u32 Offset;  // Memory offset from the base.
} arena_header;

internal ms
daco(ms aaa)
{
    
}