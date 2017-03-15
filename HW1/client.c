/**********************
 * EE209 Assignment 1 *
 **********************/

/* client.c */

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define MAX_NUM_NAME_CHARS 63
#define MAX_NUM_ID_CHARS 63
#define MAX_NUM_DIGITS   10
#define MAX_REGISTRATION 1000
/*--------------------------------------------------------------------*/
/* Pre-defined error messages */
#define ERR_UNDEF_CMD   "ERROR: Undefined Command\n"
#define ERR_UNDEF_OPT   "ERROR: Undefined Option\n"
#define ERR_NEED_OPT    "ERROR: Need More Option\n"
#define ERR_SAME_OPT    "ERROR: Multiple Same Options\n"
#define ERR_AMBIG_ARG   "ERROR: Ambiguous Argument\n"
#define ERR_INVALID_ARG "ERROR: Invalid Option Argument\n"

/*--------------------------------------------------------------------*/
enum { FALSE = 0, TRUE };


char id[MAX_REGISTRATION][MAX_NUM_ID_CHARS] = {0};
char name[MAX_REGISTRATION][MAX_NUM_NAME_CHARS] = {0};
int purchase[MAX_REGISTRATION][MAX_NUM_DIGITS] = {0};

int reg_no = 0;

typedef enum {
    C_EXIT,       /* exit */
    C_REG,        /* register */
    C_UNREG,      /* unregister */
    C_FIND,       /* find */
    C_FAIL,       /* failure */
    C_EOF         /* end of file */
} Command_T;

/*--------------------------------------------------------------------*/
/* Check whether an exit command valid.                               */
/* Input: no argument                                                 */
/* Return value: TRUE(valid) or FALSE(invalid)                        */
/*--------------------------------------------------------------------*/
static int
ValidateExitCommand(void)
{
    int c;

    /* only blank command is allowed */
    while ((c = getchar()) != EOF && c != '\n' && isspace(c))
        ;
    if (c == EOF)
        exit(0);
    if (c != '\n' && c != EOF) {
        fprintf(stderr, "%s", ERR_UNDEF_OPT);

        /* eat the line */
        while ((c = getchar()) != EOF && c != '\n')
            ;
        if (c == EOF)
            exit(0);
        return FALSE;
    }
    return TRUE;
}


/*--------------------------------------------------------------------*/
/* Eat all the space except until other characters                     */
/* Input: no argument                                                 */
/* Return value: TRUE(valid) or FALSE(invalid)                        */
/*--------------------------------------------------------------------*/
static int eatSpace(){
    int c;
    while (( c = getchar()) != EOF && isspace(c))
        ;
    if (c == EOF)
        exit(0);
    return c;
}

/*--------------------------------------------------------------------*/
/* Eat one character and return it unless EOF                         */
/*--------------------------------------------------------------------*/
char getValid_c(){
    int c;
    c = getchar();
    if (c == EOF){
        exit(0);
    } else {
        return c;
    }
}

/*--------------------------------------------------------------------*/
/* Check the input c is 'i' or 'n' or 'p'                             */
/* Input: c                                                           */
/* Return value: TRUE(valid) or FALSE(invalid)                        */
/*--------------------------------------------------------------------*/
int is_inp(c){
    if(c == 'i' || c == 'n' || c == 'p'){
        return TRUE;
    } else {
        return FALSE;
    }

}


/*--------------------------------------------------------------------*/
/* Check whether a reg(register) command valid.                       */
/* Input: no argument                                                 */
/* Return value: TRUE(valid) or FALSE(invalid)                        */
/*--------------------------------------------------------------------*/
static int
ValidateRegisterCommand(void)
{
    int c;
    int hasId = FALSE; int hasName = FALSE; int hasPurchase = FALSE;

    /* only blank command is allowed */
    c = eatSpace();

    while( !(hasId && hasName && hasPurchase) ){
        if (c == '-'){
            c = getValid_c();

            if (is_inp(c)){
                switch (c){
                    case 'i':
                        if (hasId){
                            fprintf(stderr, "%s", ERR_SAME_OPT);
                        }else{
                            if(verify_record_Id()){
                                hasId = TRUE;
                            } else {
                                fprintf(stderr, "%s", ERR_UNDEF_OPT);
                                return FALSE;
                            }
                            break;
                        }
                    case 'n': break; //verifyName(); hasName = TRUE; break;
                    case 'p': break;//verifyPurchase(); hasPurchase = TRUE; break;
                    default : break;
                }
            }
            else {
                fprintf(stderr, "%s", ERR_UNDEF_OPT);
                break;
            }
        }
        c = getValid_c();
    }



    fprintf(stderr, "%s", ERR_UNDEF_OPT);

    return FALSE;
}

/*---------------------------------------------------------*/
/* check whether input character id is legal               */
/* Input : c (a character of id)                           */
/* Return : TRUE or FALSE                                  */
/*                                                         */
/* a legal id consists of alphabets, digits, hyphens('-'), */
/* underscores('_') and periods('.').                      */

int isLegalId(char c){
    if (isalpha(c) || isdigit(c) || c == "-" || c == "_" || c == "."){
        return TRUE;
    }
    return FALSE;
}

//verify and record id
int verify_record_Id(){
    int input_length = 0;
    char c;
    c = eatSpace();
    if ( c == '\n' ){return FALSE;}
    while(input_length < MAX_NUM_ID_CHARS && isLegalId(c) ){
        id[reg_no][input_length] = c ;
        input_length++;
        c = getValid_c();
    }
    if (input_length >= MAX_NUM_ID_CHARS){
        return FALSE;
    }
    if (isspace(c)){
        printf("record!\n");
        return TRUE;
    }

}



/*--------------------------------------------------------------------*/
/* Check whether an unreg(unregister) or a find(search) command valid.*/
/* Input: no argument                                                 */
/* Return value: TRUE(valid) or FALSE(invalid)                        */
/*--------------------------------------------------------------------*/
static int
ValidateUnregisterOrFindCommand(void)
{
    /* TODO: Implement this function. */
    return FALSE;
}
/*--------------------------------------------------------------------*/
/* Read the first word, and figure out and return the command type.   */
/* Input: no argument                                                 */
/* Return value: Command_T value                                      */
/*  - In case of an error, it eats the entire line and returns C_FAIL */
/*  - In case there's no more input (EOF), it returns C_EOF           */
/*--------------------------------------------------------------------*/
static Command_T
GetCommandType(void)
{
    Command_T type = C_FAIL;
    const char *cmds[] = {
        "exit",   /* exit */
        "reg",    /* reg */
        "unreg",  /* unreg */
        "find",   /* find */
    };
    int i, len;
    int c;

    /* eat space */
    while ((c = getchar()) != EOF && c != '\n' && isspace(c))
        ;

    switch (c) {
        case '\n':return C_FAIL;  /* no command */
        case EOF: return C_EOF;   /* no more input */
        case 'e': type = C_EXIT;  break;
        case 'r': type = C_REG;   break;
        case 'u': type = C_UNREG; break;
        case 'f': type = C_FIND;  break;
        default:
            fprintf(stderr, "%s", ERR_UNDEF_CMD);
            goto EatLineAndReturn;
    }

    /* see the rest of the command chars actually match */
    len = strlen(cmds[type]);
    for (i = 1; i < len; i++) {
        c = getchar();
        if (c == '\n' || c == EOF) {   /* line finished too early */
            fprintf(stderr, "%s", ERR_UNDEF_CMD);
            return (c == EOF) ? C_EOF : C_FAIL;
        }
        if (c != cmds[type][i]) {    /* wrong command */
            fprintf(stderr, "%s", ERR_UNDEF_CMD);
            goto EatLineAndReturn;
        }
    }

    /* check the following character of a command */
    c = getchar();
    if (c != '\n' && isspace(c)) {
        return type;
    } else if (c == '\n' || c == EOF) {
        /* only exit can be followed by '\n' */
        if (type != C_EXIT)
            fprintf(stderr, "%s", ERR_NEED_OPT);
        if (c == EOF)
            return C_EOF;
        else
            ungetc(c, stdin);
        if (type == C_EXIT)
            return type;
    } else {
        fprintf(stderr, "%s", ERR_UNDEF_CMD);
    }

EatLineAndReturn:
    while ((c = getchar()) != EOF && (c != '\n'))
        ;
    return (c == EOF) ? C_EOF : C_FAIL;
}
/*--------------------------------------------------------------------*/
int
main(int argc, const char *argv[])
{
    Command_T command;
    int res;
    printf("======================================\n" \
           "  Customer Manager Program\n" \
           "======================================\n\n");

    /* start prompt */
    while (TRUE) {
        printf("\n> ");

        /* check command type */
        command = GetCommandType();

        /* command validation */
        switch (command) {
            case C_EOF:
                return 0;
            case C_FAIL:
                res = FALSE;
                break;
            case C_EXIT:
                res = ValidateExitCommand();
                break;
            case C_REG:
                res = ValidateRegisterCommand();
                break;
            case C_FIND:
                res = ValidateUnregisterOrFindCommand();
                break;
            case C_UNREG:
                res = ValidateUnregisterOrFindCommand();
                break;
            default:
                assert(0); /* can't reach here */
                break;
        }

        if (res == FALSE) {
            /* validation fail */
            continue;
        }

        /* command functionalities */
        switch (command) {
            case C_EXIT:
                exit(0);
                return 0;
            default:
                /* This will be expanded in assignment 3. */
                break;
        }
    }
    assert(0);  /* can't reach here */
    return 0;
}
