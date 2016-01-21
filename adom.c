#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include <readline/readline.h>
#include <readline/history.h>

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

void addElem(char*in)
{
    item  * list = NULL;
    list = (item *)malloc(sizeof(item));
    strcpy(list->comm , in);
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

char * wordAfter(char *fl,char *com)
{
    int fll = strlen(fl);
    char *st = strstr(com,fl);

    if(st == NULL)
    {
        //printf("_%s_",st);
        return '\0';
    }

    st+=fll;

    if(st[0]=='\0' || st[0]=='\n' || st[0]=='-')
        return 1;

    int i=0;
    if(st[0]=='\"')
    {
        st++;
        for(i=0; i<strlen(st); i++)
            if(st[i]=='\"')
            {
                break;
            }
    }
    else
    {
        for(; i<strlen(st); i++)
            if(st[i]==' ')
                break;
    }

    char * word = (char*) malloc((i+1) * sizeof(char));
    strncpy(word,st,i);
    word[i]='\0';
    return word ;
}

int strToInt(char *s)
{
    int r=0;
    int l= strlen(s);
    int i,z=1;
            printf("%s| %d \n",s,l);

    for(i=l-1 ; i>0 ; i--)
    {
        if(s[i]>'0'&&s[i]<'9')
        {
            int n=s[i]-'0';
            printf("%d_",n);
            r+=n*pow(10,z);
            z++;
        }
        if(s[i]=='-')
        {
            r*=-1;
        }
    }
}

int processComm(char *in)
{
    if(strcmp(in,"!hst")==0)
    {
        printf(KRED);
        printf( "History : %d \n" KWHT,end->index);
        printAll();
        printf("\n");
        return 1;
    }

    if(strcmp(in,"!hst")==0)
    {
        printf(KRED "Commands :\n" KNRM);
        printf("!exit - quits the console\n" KNRM);
        printf("!hst - shows the current sesion history\n" KNRM);
    }

    char *head = wordAfter("!head ",in) ;
    if(head)
    {
        int pr=0 , type=1 , lun=10;
        char *v = wordAfter(" -v ",in) ;
        if(v)pr=1;
        char *q = wordAfter(" -q ",in) ;
        if(q)pr=-1;

        char *n = wordAfter(" -n ",in) ;
        if(n){
//            printf(" %s /",n);
            printf(" %d ",strToInt(n));
        }

    }

    //!nl adom.c -s
    char *nl = wordAfter("!nl ",in) ;
    if( nl )
    {
        char *sep = "\t";
        char *s = wordAfter(" -s ",in);
        if(s)
        {
            sep=s;
        }
        char *pag = "\:\:\:";
        char *d= wordAfter(" -d ",in);
        if(d)
        {
            pag=d;
        }

        char * line = NULL;
        size_t len = 0;
        ssize_t read;

        FILE * fp= fopen(nl, "r");
        if (fp == NULL)
        {
            printf("No sutch file : %s",nl);
            return 0;
        }
        int i=1,npg=1;
        printf("\t---------------,page %d---------------\n\n",npg);
        while ((read = getline(&line, &len, fp)) != -1)
        {
            //printf("Retrieved line of length %zu :\n", read);
            if(strlen(line)>1)
            {
                printf("%d%s%s",i,sep, line);
                i++;
            }
            else
            {
                printf("%s", line);
            }

            if(strstr(line, pag) != NULL)
            {
                i=1;
                npg++;
                printf("\n\t---------------,page %d---------------\n\n",npg);

            }


        }

        fclose(fp);
        if (line)
            free(line);
    }

    return 0;
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

/* Read a string, and return a pointer to it.  Returns NULL on EOF. */
char * rl_gets ()
{
    /* A static variable for holding the line. */
    char *in = (char *)NULL;
    /* If the buffer has already been allocated, return the memory
       to the free pool. */
    if (in)
    {
        free (in);
        in = (char *)NULL;
    }

    /* Get a line from the user. */
    in = readline (SIM_COMM "\033[s" );

    /* If the line has any text in it, save it on the history. */
    if (in && *in)
    {
        add_history (in);
        addElem(in);
    }
    return in;
}

int main()
{
    //clear the screen  -  updated cursor movcement from http://www.tldp.org/HOWTO/Bash-Prompt-HOWTO/x361.html
    fprintf(stdout,"\033[2J" OO_COMM);
    char *in;
    do
    {
        in = rl_gets();
        processComm(in);
    }
    while(strcmp(in,EXIT_COMM));

    return 0;
}
