// (c) ZaKlaus 2016; All Rights Reserved

#include"hftw.h"
#include<stdio.h>

#define BUFFERSIZE 4096

typedef struct
{
    memory_arena Category;
    memory_arena Name;
    memory_arena Desc;
    memory_arena Expl;
} doc_elem;

LinkedList(doc_elem)

char * getline(void) {
    char * line = malloc(100), * linep = line;
    size_t lenmax = 100, len = lenmax;
    int c;
    
    if(line == NULL)
        return NULL;
    
    for(;;) {
        c = fgetc(stdin);
        if(c == EOF)
            break;
        
        if(--len == 0) {
            len = lenmax;
            char * linen = realloc(linep, lenmax *= 2);
            
            if(linen == NULL) {
                free(linep);
                return NULL;
            }
            line = linen + (line - linep);
            linep = linen;
        }
        
        if((*line++ = (char)c) == '\n')
            break;
    }
    *line = '\0';
    return linep;
}

int
main(int argc,const char **argv)
{
     char *Title = "Generated with docgen!";
    char *Overview = "<p>Empty</p>";
    if(argc > 1)
    {
        FILE *Ow = fopen(argv[1], "r");
        if(!Ow)return(3);
        fseek(Ow, 0, SEEK_END);
        int size = ftell(Ow);
        rewind(Ow);
         Title= PlatformMemAlloc(sizeof(char)*size+1);
        fread(Title, sizeof(char), size, Ow);
        Title[size] = 0;
        fclose(Ow);
    }
    if(argc > 2)
    {
        FILE *Ow = fopen(argv[2], "r");
        if(!Ow)return(4);
        fseek(Ow, 0, SEEK_END);
        int size = ftell(Ow);
        rewind(Ow);
        Overview = PlatformMemAlloc(sizeof(char)*size);
        fread(Overview, sizeof(char), size, Ow);
        fclose(Ow);
    }
    
    char *doc = getline();
    Node_doc_elem *Head = 0, *Tail = 0;
    
    // NOTE(zaklaus): Handle doc input
    {
        char *Ptr = doc;
        b32 IsNewNode = 1, ShouldEnd = 0;
        doc_elem *docs = PlatformMemAlloc(sizeof(doc_elem)), dummy = {0};
        
        {
            docs->Name.Base = 0;
            docs->Category.Base = 0;
            docs->Expl.Base = 0;
            docs->Desc.Base = 0;
        }
        
        do
        {
            if(!Ptr)break;
            if(*Ptr == '#') ++Ptr;
            if(!Ptr)break;
            
            char C = *Ptr++; Ptr++;
            char Buffer[1024];
            switch(C)
            {
                case '!':
                {
                    ShouldEnd = 1;
                    goto node_decl;
                }break;
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
                            Tail = AddNode_doc_elem(Tail, *docs);
                        }
                        
                        if(ShouldEnd)
                            goto html;
                        
                        IsNewNode = 1;
                        docs = PlatformMemAlloc(sizeof(doc_elem));
                        
                        {
                            docs->Name.Base = 0;
                            docs->Category.Base = 0;
                            docs->Expl.Base = 0;
                            docs->Desc.Base = 0;
                        }
                    }
                    else IsNewNode = 0;
                    
                    s32 Size = 0;
                    do
                    {
                        Buffer[Size] = *Ptr;
                        ++Size;
                    }
                    while(Ptr && *++Ptr != '#');
                    
                      
                    ArenaBuild(&docs->Name, Size);
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
                    while(Ptr && *++Ptr != '#');
                    
                    ArenaBuild(&docs->Desc, Size);
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
                    while(Ptr && *++Ptr != '#');
                    
                    ArenaBuild(&docs->Expl, Size);
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
                    while(Ptr && *++Ptr != '#');
                    
                    ArenaBuild(&docs->Category, Size);
                    ArenaPushAndNullTerminate(&docs->Category, Size, Buffer);
                }break;
            }
        }
        while(Ptr);
    }
    html:
    // NOTE(zaklaus): HTML
    {
        printf("<html>");
        // NOTE(zaklaus): HEAD
        {
            // NOTE(zaklaus): TITLE
            {
                printf("<title>%s</title>", Title);
            }
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
                    printf("%s", Overview);
                    printf("</div>");
                }
                
                // NOTE(zaklaus): LISTING
                {
                    printf("<h1>$ Index</h1>");
                    printf("<ul>");
                    
                    // TODO(zaklaus): CATEGORY
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
                            printf("<li> <a href=\"#%s\">%s</a></li>", E->Value.Name.Base, E->Value.Name.Base);
                        }
                    }
                    
                    printf("</ul>");
                }
                
                // NOTE(zaklaus): CONTENT
                {
                    printf("<h1>$ Index</h1>");
                    printf("<ul>");
                    
                    // TODO(zaklaus): CATEGORY
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
                                printf("<div id=\"%s\" class\"doc\">", E->Value.Name.Base);
                                
                                printf("<h2>%s</h2>", E->Value.Name.Base);
                                
                                if(E->Value.Desc.Base)
                                {
                                    printf("<p>%s</p>",E->Value.Desc.Base);
                                }
                                
                                if(E->Value.Expl.Base)
                                {
                                    printf("<b>Example:</b> <code>%s</code>",E->Value.Expl.Base);
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