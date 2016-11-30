// (c) ZaKlaus 2016; All Rights Reserved

#define HANDMADE_SLOW

#include"hftw.h"
#include<stdio.h>

#define BUFFERSIZE 4096

typedef struct
{
    memory_arena Category;
    memory_arena Name;
    memory_arena Desc;
    memory_arena Expl;
    memory_arena Sig;
    memory_arena Ret;
    b32 Separate;
} doc_elem;

LinkedList(doc_elem)

void
reset_docs(doc_elem *docs)
{
    docs->Name.Base = 0;
    docs->Category.Base = 0;
    docs->Expl.Base = 0;
    docs->Desc.Base = 0;
    docs->Sig.Base = 0;
    docs->Ret.Base = 0;
    docs->Separate = 0;
}

int
main(int argc,const char **argv)
{
     char *Title = "Generated with docgen!";
    int TitleLen = 22;
    char *Overview = "<p>Empty</p>";
    int OverviewLen = 12;
    char *doc = "0 Empty#!";
    int DataLen = 10;
    if(argc > 1)
    {
        FILE *Ow = fopen(argv[1], "r");
        if(!Ow)return(3);
        fseek(Ow, 0, SEEK_END);
        int size = ftell(Ow);
        rewind(Ow);
        doc= PlatformMemAlloc(sizeof(char)*size+1);
        fread(doc, sizeof(char), size, Ow);
        doc[size] = 0;
        DataLen = size;
        fclose(Ow);
    }
    if(argc > 2)
    {
        FILE *Ow = fopen(argv[2], "r");
        if(!Ow)return(3);
        fseek(Ow, 0, SEEK_END);
        int size = ftell(Ow);
        TitleLen = size;
        rewind(Ow);
         Title= PlatformMemAlloc(sizeof(char)*size+1);
        fread(Title, sizeof(char), size, Ow);
        Title[size] = 0;
        fclose(Ow);
    }
    if(argc > 3)
    {
        FILE *Ow = fopen(argv[3], "r");
        if(!Ow)return(4);
        fseek(Ow, 0, SEEK_END);
        int size = ftell(Ow);
        OverviewLen = size;
        rewind(Ow);
        Overview = PlatformMemAlloc(sizeof(char)*size);
        fread(Overview, sizeof(char), size, Ow);
        Overview[size] = 0;
        fclose(Ow);
    }
    
    Node_doc_elem *Head = 0, *Tail = 0;
    
    // NOTE(zaklaus): Handle doc input
    {
        char *Ptr = doc;
        b32 IsNewNode = 1, ShouldEnd = 0;
        doc_elem *docs = PlatformMemAlloc(sizeof(doc_elem)), dummy = {0};
        
        reset_docs(docs);
        
        do
        {
            if(!Ptr)break;
            if(*Ptr == '#') ++Ptr;
            if(!Ptr)break;
            
            char C = *Ptr++; Ptr++;
            char *Buffer = PlatformMemAlloc(DataLen);
            if(!*Ptr)
            {
                ShouldEnd = 1;
                goto node_decl;
            }
            switch(C)
            {
                case '0':
                {
                    if(!IsNewNode)
                    {
                        node_decl:
                        if(!Head)
                        {
                            Head = NewNode_doc_elem(dummy);
                            Tail = NewNode_doc_elem(*docs);
                            Head->Next = Tail;
                        }
                        else
                        {
                            Tail->Next = NewNode_doc_elem(*docs);
                            Tail = Tail->Next;
                        }
                        
                        if(ShouldEnd)
                            goto html;
                        
                        IsNewNode = 1;
                        docs = PlatformMemAlloc(sizeof(doc_elem));
                        
                        reset_docs(docs);
                    }
                    IsNewNode = 0;
                    
                    s32 Size = 0;
                    do
                    {
                        Buffer[Size] = *Ptr;
                        ++Size;
                    }
                    while(*Ptr && *++Ptr != '#');
                    
                    ArenaBuild(&docs->Name, Size);
                    docs->Name.Flags = ArenaFlag_AllowRealloc;
                    ArenaPushAndNullTerminate(&docs->Name, Size, Buffer);
                }break;
                case '1':
                {
                    s32 Size = 0;
                    do
                    {
                        Buffer[Size] = *Ptr;
                        ++Size;
                    }
                    while(*Ptr && *++Ptr != '#');
                    
                    ArenaBuild(&docs->Desc, Size);
                    docs->Desc.Flags = ArenaFlag_AllowRealloc;
                    ArenaPushAndNullTerminate(&docs->Desc, Size, Buffer);
                }break;
                
                case '2':
                {
                    s32 Size = 0;
                    do
                    {
                        Buffer[Size] = *Ptr;
                        ++Size;
                    }
                    while(*Ptr && *++Ptr != '#');
                    
                    ArenaBuild(&docs->Expl, Size);
                    docs->Expl.Flags = ArenaFlag_AllowRealloc;
                    ArenaPushAndNullTerminate(&docs->Expl, Size, Buffer);
                }break;
                
                case '3':
                {
                    s32 Size = 0;
                    do
                    {
                        Buffer[Size] = *Ptr;
                        ++Size;
                    }
                    while(*Ptr && *++Ptr != '#');
                    
                    ArenaBuild(&docs->Category, Size);
                    docs->Category.Flags = ArenaFlag_AllowRealloc;
                    ArenaPushAndNullTerminate(&docs->Category, Size, Buffer);
                }break;
                
                case '4':
                {
                    s32 Size = 0;
                    do
                    {
                        Buffer[Size] = *Ptr;
                        ++Size;
                    }
                    while(*Ptr && *++Ptr != '#');
                    
                    ArenaBuild(&docs->Sig, Size);
                    docs->Sig.Flags = ArenaFlag_AllowRealloc;
                    ArenaPushAndNullTerminate(&docs->Sig, Size, Buffer);
                }break;
                
                case '5':
                {
                    s32 Size = 0;
                    do
                    {
                        Buffer[Size] = *Ptr;
                        ++Size;
                    }
                    while(*Ptr && *++Ptr != '#');
                    
                    ArenaBuild(&docs->Ret, Size);
                    docs->Ret.Flags = ArenaFlag_AllowRealloc;
                    ArenaPushAndNullTerminate(&docs->Ret, Size, Buffer);
                }break;
                
                case '6':
                {
                    docs->Separate = 1;
                }break;
            }
        }
        while(*Ptr);
    }
    html:
    
    char *Style = "body{background-color:#f5f5f5;font-family:sans-serif}.Page{width:800px;margin:0 auto}p{font-size:15px;line-height:.4}h1,h2{color:green}a{text-decoration:none;color:green}a:hover{background-color:#faebd7}ul{list-style:none}pre{font-family:monospace;background-color:#fdf5e6;padding:10px;border:1px solid grey}table{border:1px solid grey;background-color:#fdf5e6;border-collapse:collapse}tr{border-bottom:1px solid grey}th{text-align:left;background-color:#ffe4c4;padding:5px;border-right:1px solid grey}td{padding:5px;border-right:1px solid grey}";
    
    // NOTE(zaklaus): HTML
    {
        printf("<html>");
        // NOTE(zaklaus): HEAD
        {
            printf("<head>");
            // NOTE(zaklaus): TITLE
            {
                printf("<title>%.*s</title>", TitleLen, Title);
                printf("<style>%s</style>", Style);
            }
            printf("</head>");
        }
         u8 *CategId = 0;
        
        // NOTE(zaklaus): BODY
        {
            printf("<body>");
            // NOTE(zaklaus): PAGE
            {
                printf("<div class=\"Page\">");
                
                // NOTE(zaklaus): OVERVIEW
                {
                    printf("<div class=\"Overview\">");
                    printf("<h1>$ Overview</h1>");
                    printf("%.*s", OverviewLen, Overview);
                    printf("</div>");
                }
                
                // NOTE(zaklaus): LISTING
                {
                    printf("<h1>$ Index</h1>");
                    printf("<ul>");
                    
                    // NOTE(zaklaus): CATEGORY
                    {
                        for(Node_doc_elem *E = Head->Next;
                            E;
                            E = E->Next)
                        {
                            if(E->Value.Category.Base && E->Value.Category.Base != CategId)
                            {
                                CategId = E->Value.Category.Base;
                                printf("<h2>%s</h2>", E->Value.Category.Base);
                            }
                            printf("<li> <a href=\"#%s\">%s</a> &nbsp; <small>%s</small></li>", E->Value.Name.Base, E->Value.Name.Base, E->Value.Desc.Base);
                            if(E->Value.Separate)
                            {
                                printf("<br>");
                            }
                        }
                    }
                    
                    printf("</ul>");
                }
                
                // NOTE(zaklaus): CONTENT
                {
                    printf("<h1>$ Content</h1>");
                    printf("<ul>");
                    
                    // NOTE(zaklaus): CATEGORY
                    {
                        for(Node_doc_elem *E = Head->Next;
                            E;
                            E = E->Next)
                        {
                            if(E->Value.Category.Base && E->Value.Category.Base != CategId)
                            {
                                CategId = E->Value.Category.Base;
                                printf("<h1>$ %s</h1>", E->Value.Category.Base);
                            }
                            
                                // NOTE(zaklaus): DOC
                            {
                                printf("<div id=\"%s\" class=\"doc\">", E->Value.Name.Base);
                                
                                printf("<h2>%s</h2>", E->Value.Name.Base);
                                
                                if(E->Value.Sig.Base)
                                {
                                    printf("<pre>%s</pre>", E->Value.Sig.Base);
                                }
                                
                                if(E->Value.Desc.Base)
                                {
                                    printf("<p>%s</p>",E->Value.Desc.Base);
                                }
                                
                                if(E->Value.Expl.Base)
                                {
                                    printf("<b>Example:</b> <code>%s</code>",E->Value.Expl.Base);
                                }
                                
                                if(E->Value.Ret.Base)
                                {
                                    printf("<p><b>Returns:</b> <i>%s</i></p>",E->Value.Ret.Base);
                                }
                                
                                printf("</div>");
                            }
                        }
                    }
                    
                    printf("</ul>");
                }
                
                printf("</div>");
            }
            printf("</body>");
        }
        printf("</html>");
    }
    
    return(0);
}