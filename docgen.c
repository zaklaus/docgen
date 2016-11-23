// (c) ZaKlaus 2016; All Rights Reserved

#include"hftw.h"
#include<stdio.h>

typedef enum
{
    None,
    Name,
    Desc,
    Expl,
    Categ,
} Doc_States;

int
main(int argc, char **argv)
{
    if(argc < 2)
    {
        fprintf(stderr, "Argument not passed!\n");
        return(1);
    }
    FILE *File = fopen(argv[1], "r");
    
    if(!File)
    {
        fprintf(stderr, "Can't load file: %s!\n", argv[1]);
        return(2);
    }
    
    {
        // NOTE(zaklaus): No need for damn whole-File-read BS.
        char *Ptr = 0;
        char Buffer[256], FileData[256];
        size_t Line = 3;
        
        while(fread(FileData, sizeof(char), 256, File))
        {
            Ptr = FileData;
            size_t Idx = 0;
            b32 ShouldCopy = 0;
            u8 DocState = None;
            u8 ScopeLevel = 0;
            do
            {
                if(StringsAreEqualAB(4, (Ptr), 4, "doc("))
                {
                    Assert(!Idx && !ShouldCopy);
                    ShouldCopy = 1;
                    DocState = Name;
                    Ptr += 4;
                    //Buffer[Idx++] = *Ptr;
                }
                if(StringsAreEqualAB(11, (Ptr), 11, "doc_string("))
                {
                    Assert(!Idx && !ShouldCopy);
                    ShouldCopy = 1;
                    DocState = Desc;
                    Ptr += 11;
                    //Buffer[Idx++] = *Ptr;
                }
                if(StringsAreEqualAB(12, (Ptr), 12, "doc_example("))
                {
                    Assert(!Idx && !ShouldCopy);
                    ShouldCopy = 1;
                    DocState = Expl;
                     Ptr += 12;
                    //Buffer[Idx++] = *Ptr;
                }
                if(StringsAreEqualAB(8, (Ptr), 8, "doc_cat("))
                {
                    Assert(!Idx && !ShouldCopy);
                    ShouldCopy = 1;
                    DocState = Categ;
                    Ptr += 8;
                    //Buffer[Idx++] = *Ptr;
                }
                if(*Ptr == '(')
                {
                    ++ScopeLevel;
                }
                if(*Ptr == ')' && ScopeLevel)
                {
                    --ScopeLevel;
                }
                else if((*Ptr == ')') && ShouldCopy)
                {
                    Assert(Idx < 256);
                    Buffer[Idx] = 0;
                    
                    // NOTE(zaklaus): Handle doc
                    switch(DocState)
                    {
                        case Name:
                        {
                            fprintf(stdout, "0 %s#", Buffer);
                        }break;
                        
                        case Desc:
                        {
                            fprintf(stdout, "1 %s#", Buffer);
                        }break;
                        
                        case Expl:
                        {
                            fprintf(stdout, "2 %s#", Buffer);
                        }break;
                        
                        case Categ:
                        {
                            fprintf(stdout, "3 %s#", Buffer);
                        }break;
                    }
                    Idx = 0;
                    ZeroArray(256, Buffer);
                    *Buffer = 0;
                    ShouldCopy = 0;
                }
                if(ShouldCopy)
                {
                    if(Ptr - FileData >= 256)
                    {
                        size_t Offset = Ptr - FileData - 256;
                        ZeroArray(256, FileData);
                        *FileData = 0;
                        if(fread(FileData, sizeof(char), 256, File))
                        {
                            Ptr = FileData + Offset;
                        }
                        else 
                        {
                            printf("!");
                            return(0);
                        }
                    }
                    Buffer[Idx++] = *Ptr;
                }
            }
            while(*Ptr++);
            ZeroArray(256, FileData);
            *FileData = 0;
        }
    };
    printf("!");
    return(0);
}