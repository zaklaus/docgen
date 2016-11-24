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
    Sig,
    Ret,
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
    fseek(File, 0, SEEK_END);
    int FileSize = ftell(File);
    rewind(File);
    
    if(!File)
    {
        fprintf(stderr, "Can't load file: %s!\n", argv[1]);
        return(2);
    }
    
    {
        // NOTE(zaklaus): No need for damn whole-File-read BS.
        char *Ptr = 0;
        static char *Buffer = 0, *FileData = 0;
        Buffer = PlatformMemAlloc(FileSize);
        FileData = PlatformMemAlloc(FileSize);
        size_t Line = 3;
        
        while(fread(FileData, sizeof(char), FileSize, File))
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
                if(StringsAreEqualAB(8, (Ptr), 8, "doc_sig("))
                {
                    Assert(!Idx && !ShouldCopy);
                    ShouldCopy = 1;
                    DocState = Sig;
                    Ptr += 8;
                    //Buffer[Idx++] = *Ptr;
                }
                if(StringsAreEqualAB(8, (Ptr), 8, "doc_ret("))
                {
                    Assert(!Idx && !ShouldCopy);
                    ShouldCopy = 1;
                    DocState = Ret;
                    Ptr += 8;
                    //Buffer[Idx++] = *Ptr;
                }
                if(*Ptr == '(')
                {
                    ++ScopeLevel;
                }
                else if(*Ptr == ')' && ScopeLevel)
                {
                    --ScopeLevel;
                }
                else if((*Ptr == ')') && ShouldCopy)
                {
                    Assert(Idx < FileSize);
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
                        
                        case Sig:
                        {
                            fprintf(stdout, "4 %s#", Buffer);
                        }break;
                        
                        case Ret:
                        {
                            fprintf(stdout, "5 %s#", Buffer);
                        }break;
                    }
                    Idx = 0;
                    ZeroArray(FileSize, Buffer);
                    *Buffer = 0;
                    ShouldCopy = 0;
                }
                if(ShouldCopy)
                {
                    
                    Buffer[Idx++] = *Ptr;
                }
            }
            while(*Ptr++);
            ZeroArray(256, FileData);
            *FileData = 0;
        }
    };
	fclose(File);
	printf("!");
    return(0);
}