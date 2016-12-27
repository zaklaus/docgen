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
    Sep,
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
        b32 Ignore = 0;
        
        while(fread(FileData, sizeof(char), FileSize, File))
        {
            Ptr = FileData;
            size_t Idx = 0;
            b32 ShouldCopy = 0;
            b32 ExpectBrace = 0;
            u8 DocState = None;
            s32 ScopeLevel = 0;
            do
            {
                if(Ignore)
                {
                    if(StringsAreEqualAB(9, (Ptr), 9, "doc_hunt("))
                    {
                        Ignore = 0;
                        Ptr += 9;
                        //Buffer[Idx++] = *Ptr;
                    }
                    else
                    {
                        continue;
                    }
                }
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
                if(StringsAreEqualAB(11, (Ptr), 11, "doc_ignore("))
                {
                    Ignore = 1;
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
                if(StringsAreEqualAB(9, (Ptr), 9, "internal "))
                {
                    Assert(!Idx && !ShouldCopy);
                    ShouldCopy = 1;
                    DocState = Sig;
                    Ptr += 9;
                    //Buffer[Idx++] = *Ptr;
                }
                if(StringsAreEqualAB(8, (Ptr), 8, "typedef "))
                {
                    Assert(!Idx && !ShouldCopy);
                    ShouldCopy = 1;
                    DocState = Sig;
                    Ptr += 8;
                    ExpectBrace = 1;
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
                if(StringsAreEqualAB(8, (Ptr), 8, "doc_sep("))
                {
                    Assert(!Idx && !ShouldCopy);
                    ShouldCopy = 1;
                    DocState = Sep;
                    Ptr += 8;
                    //Buffer[Idx++] = *Ptr;
                }
                if(*Ptr == '(')
                {
                    ++ScopeLevel;
                }
                if(*Ptr == '{' && ExpectBrace)
                {
                    ++ScopeLevel;
                    ExpectBrace = 0;
                }
                 if(*Ptr == '}' && ScopeLevel)
                {
                    --ScopeLevel;
                }
                if(*Ptr == ')' && ScopeLevel)
                {
                    --ScopeLevel;
                }
                else if(((*Ptr == '{' && DocState == Sig) || *Ptr == ';' || (*Ptr == ')' && DocState != Sig)) && ShouldCopy && !ScopeLevel)
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
                        
                        case Sep:
                        {
                            fprintf(stdout, "6#");
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
        }
    };
	fclose(File);
	//printf("!");
    return(0);
}