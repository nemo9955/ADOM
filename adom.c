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
int indx=0;

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
    current=end;
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

int validChars(char *in)
{
    int l=strlen(in);
//    printf("%d",in[l-1]);

    if(in[l-1]==27||in[l-2]==27||in[l-3]==27)
        return 0;

    switch(in[l-1])
    {
    case 127:
    case 9:
        return 0;
    }

    return 1;
}

void readTerminal(char *in)
{
    in[0]='\0';
    int c =0;
    while(c!='\n')
    {
        int l = strlen(in) ;
        c=getch();
        if(c== '\b')
        {
//            printf("\033[D ");
            continue;
        }
        if(c== '\n')
        {
            printf("\n");
            break;
        }

        in[l]=c;
        in[l+1]='\0';

        if( in[l-2]==27 && in[l-1]==91 )
        {
            if(in[l-0]==65 && indx>0)//sus
                current=current->prev;
            if(in[l-0]==66&& indx>0)//jos
                current=current->next;
            if(in[l-0]==67)//dreapta
                printf("\033[C");
            if(in[l-0]==68)//stanga
                printf("\033[D");

            if((in[l-0]==66 || in[l]==65)&& indx>0)
            {
                printf("\033[u             \033[u%s",current->comm);
            }

            in[l-2]='\0';
            in[l-1]='\0';
            in[l-0]='\0';
        }
        else if(validChars(in))
            printf("%c",c);
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
        if(strlen(in)>0)
            addElem(in);
        processComm(in);
    }
    /** DONE
    *   + add commands to a list , !hst to show
    up/down show hystory
    */

    /* TODO
    *
    *|   fix backspace
    *|   separate printable charset from special/escape charset
    *   execute the history command
    *   make better : harcoded spaces added after showing shorter history command
    *   add entered history as new command
    *   add external commands support
    *
    *
    *
    *
    */

    return 0;
}
