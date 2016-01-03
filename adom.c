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
#define EXIT_COMM "!exit\n" //the comannd to initiate exit
#define OO_COMM "\033[0;0H" // sets the cursor at 0 0 in the terminal
#define SIM_COMM  KCYN " #! " KNRM // the "enter command" simbol thingy

char in[MAX_COMM];

int main()
{
    int i;
    for(i=0; i<50; i++)//some sort of clear screen, might implement it as a command like "!clscr"
        fprintf(stdout,"\n");

    fprintf(stdout,OO_COMM);//sets the cursor at terminal char coords 0 0

    while(strcmp(in,EXIT_COMM))
    {
        fprintf(stdout,SIM_COMM  );
        fgets(in,MAX_COMM,stdin);
    }

    /* TODO
    *
    *   add history
    *   add external commands
    *
    *
    *
    *
    */

    return 0;
}
//    printf("%sred", KRED);
//    printf("%sgreen", KGRN);
//    printf("%syellow", KYEL);
//    printf("%sblue", KBLU);
//    printf("%smagenta", KMAG);
//    printf("%scyan", KCYN);
//    printf("%swhite", KWHT);
//    printf("%snormal", KNRM);
