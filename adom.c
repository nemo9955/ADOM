#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "helper.c"

//praise stackoverflow.com !
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"
#define KNRM  "\x1B[0m"

//will add some more
//http://www.linuxjournal.com/article/8603

#define MAX_COMM 200 //maximum command length
#define EXIT_COMM "!exit" //the comannd to initiate exit
#define OO_COMM "\033[0;0H" // sets the cursor at 0 0 in the terminal
#define SIM_COMM  KCYN " #! " KNRM // the "enter command" simbol thingy

struct list_el
{
    int index;
    char comm[MAX_COMM];
    struct list_el *next,*prev;
};

typedef struct list_el item;
item * start, *end , *current;
int indx=0,isHist=0;

void addElem(char *p)
{
    item  * list = NULL;
    list = (item *)malloc(sizeof(item));
    strcpy(list->comm , p);
    list->index = ++indx;

    if(indx==1)
        start=list ;
    else
        end->next = list;

    list->prev=end;
    end = list ;
    end->next=start;
    start->prev =end ;
    current=start;
}

void printAll()
{
    item *list = start ;
    int count = indx ;
    while(count)
    {
        printf("%s\n",list->comm);
        list = list->next ;
        count--;
    }
}

int processComm(char * in)
{
    if(strcmp(in,"!hst")==0)
    {
        printf(KRED);
        printf( "History : %d \n" KWHT,end->index);
        printAll();
        printf("\n");
        return 1;
    }

    return 0;
}
int supress=0;
int validChars(int c)
{
    if(c==27)
    {
        supress=1;
        return 0;
    }

    if(c==91 && supress)
    {
        supress=1;
        return 0;
    }

    if(supress){
        supress--;
        return 0;
    }

    return 1;
}

void printChars(char *s)
{
    int i=0;
    printf("\n");
    for(; i<=strlen(s); i++)
        printf("%d\t",s[i]);
    printf("\n");
    for(i=0; i<=strlen(s); i++)
        printf("%c\t",s[i]);
//    printf("\n");
}

void readTerminal(char *in)
{
    in[0]='\0';
    int c =0;
    while(c!='\n')
    {
        int l = strlen(in) ;
        c=getch();
        if(c== '\n')
        {
            printf("\n");
            isHist=0;
            break;
        }
        else if(c== 127)
        {
            if(l>0)
            {
                printf("\033[D \033[D");
                in[l-1]='\0';
            }
        }
        else
        {
            in[l]=c;
            in[l+1]='\0';
            if(validChars(c))
                printf("%c",c);
        }

//        printf("_%d_",c);
        if( in[l-2]==27 && in[l-1]==91 )
        {
            if((in[l]==66 || in[l]==65) &&indx>0 )
            {
                if(isHist==0)
                {
                    isHist=1;
                in[l-2]='\0';
                in[l-1]='\0';
                in[l-0]='\0';
                    if(strlen(in)>0)
                        addElem(in);
                    end=end->prev;
                }

                if(in[l-0]==65 && indx>0)//sus
                    current=current->prev;
                if(in[l-0]==66&& indx>0)//jos
                    current=current->next;

                printf("\033[u");
                int sp=0;
                for(; sp<MAX_COMM*5; sp++)
                    printf(" ");
//                printf("\0");

                printf("\033[u%s",current->comm);
                strcpy(in,current->comm);
//                printChars(in);
//                printChars(current->comm);

            }
            else
            {
                in[l-2]='\0';
                in[l-1]='\0';
                in[l-0]='\0';
            }
        }
    }
}


int main()
{
    //clear the screen  -  updated cursor movcement from http://www.tldp.org/HOWTO/Bash-Prompt-HOWTO/x361.html
    fprintf(stdout,"\033[2J" OO_COMM);

    char in[MAX_COMM];
    while(strcmp(in,EXIT_COMM))
    {
        fprintf(stdout,SIM_COMM "\033[s" );
        readTerminal(in);
//        printChars(in);
        if(strlen(in)>0)
            addElem(in);
        processComm(in);
    }



    /** DONE
    *   + add commands to a list , !hst to show
    *   up/down show hystory
    *|   fix backspace
    *|   separate printable charset from special/escape charset
    *   execute the history command
    *   make better : harcoded spaces added after showing shorter history command
    *   add entered history as new command
    */

    /* TODO
    *
    *   add external commands support
    *
    */

    return 0;
}
