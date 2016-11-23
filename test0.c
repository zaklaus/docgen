
doc(ArenaGetAlignmentOffset)
doc_cat(Memory Arena)
doc_string(
<pre>
inline memory_index
ArenaGetAlignmentOffset(memory_arena *Arena, memory_index Alignment)
</pre>
Calculates the offset required by specified alignment.)
doc_example(ArenaGetAlignmentOffset(Arena, 8);)
inline memory_index
ArenaGetAlignmentOffset(memory_arena *Arena, memory_index Alignment)
{
    memory_index AlignmentOffset = 0;
    
    memory_index ResultPointer = (memory_index)Arena->Base + Arena->Used;
    memory_index AlignmentMask = Alignment - 1;
    if(ResultPointer & AlignmentMask)
    {
        AlignmentOffset = Alignment - (ResultPointer & AlignmentMask);
    }
    
    return(AlignmentOffset);
}

doc(ArenaDefaultParams)
doc_cat(Test Category)
doc_string(Returns default push state params.)
inline arena_push_params
ArenaDefaultParams(void)
{
    arena_push_params Params;
    Params.Flags = ArenaPushFlag_ClearToZero;
    Params.Alignment = 4;
    Params.Expectation = 0;
    Params.Tag = 0;
    return(Params);
}
