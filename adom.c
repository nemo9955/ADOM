#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

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

#define OO_COMM "\033[0;0H" // sets the cursor at 0 0 in the terminal
#define SIM_COMM  KCYN " #! " KNRM // the "enter command" simbol thingy

struct list_el
{
    int index;
    char comm[1000];
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

char * wordAfterIndex(char *fl,char *com,int ind)
{
    int fll = strlen(fl);
    char *st = strstr(com,fl);

    if(st == NULL)
        return '\0';

    if(fll>0)
        st+=fll-1;

    if(st[0]=='\0' )
        return "1";

    int i=0,down=ind;
    do
    {
        if(strlen(st) == 0)break;

        down--;
        st+=i;
        st++;//+1 to delete the leading space
//    printf("+%c+\n",st[0]);
        char chh = ' ';
        if(st[0]=='\"')
        {
            chh='\"';
            st++;
        }
        if(st[0]=='\'')
        {
            chh='\'';
            st++;
        }


        for(i=0; i<strlen(st); i++)
            if(st[i]==chh )
                break;


//        printf("+%s+\n\n",st);
    }
    while(down >= 0 );
//    printf("-%d %s-\n",i,st);

    char * word = (char*) malloc((i+1) * sizeof(char));
    strncpy(word,st,i);
    word[i]='\0';
    return word ;

}
char * wordAfter(char *fl,char *com)
{
    return wordAfterIndex(fl,com,0);
}

int strToInt(char *s)
{
    int r=0;
    int l= strlen(s);
    int i,z=0;
//            printf("%s| %d \n",s,l);

    for(i=l-1 ; i>=0 ; i--)
    {
        if(s[i]>='0'&&s[i]<='9')
        {
            int n=s[i]-'0';
//            printf("%d_",n);
            r+=n*( (int)pow(10,z));
            z++;
        }
        if(s[i]=='-')
            r*=-1.0;
    }
    return (int)r;
}

int clearCOMM(char *in)
{
    if(strcmp(in,"!clear")==0)
    {
        printf("\033[2J" OO_COMM);
        return 1;
    }
    return 0;
}

int historyCOMM(char *in)
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
int helpCOMM(char *in)
{

    if(strcmp(in,"!help")==0)
    {
        printf(KRED "Commands :\n" KNRM);
        printf("Format : command [file 1] ... [file n] [param 1] ... [param n]\n" );
        printf("!exit - quits the console\n" );
        printf("!hst - shows the current sesion history\n" );
        printf("!clear - clears the console\n" );

        printf("!nl - numbers the lines\n" );
        printf("\t-s - number separator\n" );
        printf("\t-d - page delimiter\n" );

        printf("!head - prints first part of file\n" );
        printf("\t-n - first lines, \"-\" excludes the last lines\n" );
        printf("\t-c - first bytes, \"-\" excludes the last lines\n" );
        printf("\t-q - don't print file name\n" );
        printf("\t-v - always print file name\n" );

        return 1;
    }

    return 0;
}


int chrootCOMM(char *in)
{

    char *chr = wordAfter("!chroot ",in) ;
    if(chr)
    {
        int result = chroot(chr);

        if(result == -1)
        {
            printf("Chroot a esuat :%s.\n",chr);
        }
        return 1;
    }

    return 0;
}


int headCOMM(char *in)
{

    char *head = wordAfter("!head ",in) ;
    if(head)
    {
        int pr=0 , type=1 , lun=10 ;
        char *q = wordAfter(" -q",in) ;
        char *v = wordAfter(" -v",in) ;
        if(q&&v)
        {
            printf("only one of attributes -v and -q can be used on same command !\n");
            return 1;
        }
        if(v || !strchr("- \n\0",wordAfterIndex("!head ",in,1)[0] )  ) pr=1;
        if(q)pr=0;

        char *n = wordAfter(" -n ",in) ;
        char *c = wordAfter(" -c ",in) ;
        if(n&&c)
        {
            printf("only one of attributes -n and -b can be used on same command !\n");
            return 1;
        }

        if(n)
            lun = strToInt(n);

        if(c)
        {
            type=2;
            lun = strToInt(c);
        }


        int wrd =0 ;
        while(head = wordAfterIndex("!head ",in,wrd++))
        {
            FILE *fp = fopen(head, "rb");

            if (fp == NULL)
            {
//                printf("========%s\n",head);
                break;
            }
//            printf("++%s++",head);

            if(pr)
                printf("\t =======> %s <=======\n\n",head);

            fseek(fp, 0, SEEK_END);
            long endByte = ftell(fp);
            fseek(fp, 0, SEEK_SET);

            if(type==2)
            {

//                printf("_%d_\n",lun);
//                printf("_%lu_\n",endByte);
                if(lun > 0)endByte=lun;
                if(lun < 0) endByte+=lun;
                if(endByte<0)endByte=0;

//                printf("_%lu_\n",endByte);

                char *string = malloc(endByte + 1);
                fread(string, endByte, 1, fp);
                string[endByte] = 0;
                printf("%s\n",string);
            }


            if(type==1)
            {
                char *s = malloc(endByte + 1);
                fread(s, endByte, 1, fp);
                s[endByte] = '\0';
//                printf("%s\n",head);
                int i=0,con=0;
                for(i=0; i<strlen(s); i++)
                    if(s[i] == '\n')
                        con++;

                if(lun > 0)con=lun;
                if(lun < 0) con+=lun;
                if(con<0)con=0;

                i=0;
                while(con > 0 )
                {
                    if(s[i]=='\0')break;
                    printf("%c",s[i++]);
                    if(s[i]=='\n')con--;
                }
                printf("\n");

            }
            fclose(fp);
        }
        return 1;
    }
    return 0;
}

int nlCOMM(char *in)
{
    char *nl = wordAfter("!nl ",in) ;
    if( nl )
    {
        char *sep = "\t";
        char *s = wordAfter(" -s ",in);
        if(s)
            sep=s;
        char *pag = "\:\:\:";
        char *d= wordAfter(" -d ",in);
        if(d)
            pag=d;


        int wrd =0 ;
        while(nl = wordAfterIndex("!nl ",in,wrd++))
        {

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
                    printf("%d%s%s",i++,sep, line);
                else
                    printf("%s", line);
                if(strstr(line, pag) != NULL)
                {
                    i=1;
                    printf("\n\t---------------,page %d---------------\n\n",npg++);
                }
            }
            fclose(fp);
            if (line)
                free(line);
            printf("\n");
        }
        return 1;
    }

    return 0;
}

int processComm(char *in)
{
    if(clearCOMM(in))return 1;
    if(historyCOMM(in))return 1;
    if(helpCOMM(in))return 1;
    if(headCOMM(in))return 1;
    if(nlCOMM(in))return 1;
    if(chrootCOMM(in))return 1;

    return 0;
}

char* concatenate(char *s1, char *s2)
{
    char *r = malloc(strlen(s1)+strlen(s2)+1);
    strcpy(r, s1);
    strcat(r, s2);
    return r;
}

char* getFirstWord(char*in)
{
    int i;
    for(i=0; i<strlen(in); i++)
        if(in[i]==' ')
            break;
    char * word = malloc(i+1);
    strncpy(word,in,i);
    word[i]=0;
    return word ;
}

void externalComm(char *ex)
{
    pid_t pid;
    int	ppe[2];//pipe pentru a prelua output-ul procesului nou creat

//    if(pipe(ppe))
//    {
//        printf("Eroare la pipe(ppe)\n");
//        return ;
//    }

    if( (pid=fork()) == -1)
    {
        printf("Eroare la fork()\n");
        return ;
    }

    //parintele
    if(pid>0)
    {
        int stat ;
//        dup2(ppe[1],stdout);//schimbam capetele pipe-ului
//        close(ppe[0]);//inchidem inputul de la pipe
        waitpid (pid, &stat, 0);	//asteapta dupa copil !
//        printf("/nchild stat : %d\n",stat);
    }
    else//copchilul
    {
//        dup2(ppe[0],stdin);
//        close(ppe[1]);//inchidem output-ul de la pipe

        char* com = getFirstWord(ex);

        execl( concatenate("/bin/",com), ex  ,NULL  );
        execl( concatenate("",com), ex  ,NULL  );
        printf("FallBack !\n");

        execl("/bin/bash","/bin/bash", "-c",ex,NULL);
        execl("/bin/sh","/bin/sh", "-c",ex,NULL);

        printf("Eroare la execl()\n");
        exit(42);
    }
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

char * readLine ()
{
    char *in ;

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
        in = readLine();
        if(!processComm(in))
            externalComm(in);
    }
    while(strcmp(in,"!exit"));

    return 0;
}
