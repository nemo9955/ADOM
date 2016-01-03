#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
    struct list_el * next;
};

typedef struct list_el item;
item * start, *end ;
int indx;

void addElem(char *p)
{
    item  * list = NULL;
    list = (item *)malloc(sizeof(item));
//    list->comm = (char *)malloc(sizeof(char)*strlen(p));
    strcpy(list->comm , p);
    list->index = ++indx;
//    list->next  = NULL;

    if(indx==1)
        start=list ;
    else
        end->next = list;

    end = list ;
}

void printAll()
{
    item *list = start ;

    while(list)
    {
        printf("%s\n",list->comm);
        list = list->next ;
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

int main()
{
    //clear the screen  -  updated cursor movcement from http://www.tldp.org/HOWTO/Bash-Prompt-HOWTO/x361.html
    fprintf(stdout,"\033[2J" OO_COMM);

    char in[MAX_COMM];
    while(strcmp(in,EXIT_COMM))
    {
        fprintf(stdout,SIM_COMM  );
        fgets(in,MAX_COMM,stdin);
        in[strlen(in)-1] = '\0';
        addElem(in);
        switch(processComm(in))
        {
        }
    }

    /* TODO
    *
    *   + add commands to a list , !hst to show
    *   add history
    *   add external commands support
    *
    *
    *
    *
    */

    return 0;
}
