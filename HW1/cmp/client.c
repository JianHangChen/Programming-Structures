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


char id[MAX_REGISTRATION][MAX_NUM_ID_CHARS] ;
char name[MAX_REGISTRATION][MAX_NUM_NAME_CHARS] ;
int purchase[MAX_REGISTRATION][MAX_NUM_DIGITS] ;

int reg_no = 0;

/*----------------------------------------------------------------------*/
int verify_record_Id();
int verify_record_name();
int verify_record_purchase();
int isLegalId(char c);
int isLegalName(char c);
int isLegalPurchase(char c);

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
/* Eat all the characters except until meet \n                     */
/* Input: no argument                                                 */
/* Return value: the int number of the character                      */
/*--------------------------------------------------------------------*/
static int eatLine(){
    int c;
    // eat line and return
    while ((c = getchar()) != EOF && (c != '\n'))
        ;
    if (c == EOF)
        exit(0);
    return c;
}


/*--------------------------------------------------------------------*/
/* Eat all the space except until other characters                     */
/* Input: no argument                                                 */
/* Return value: the int number of the character                      */
/*--------------------------------------------------------------------*/
static int eatSpace(){
    int c;
    while (( c = getchar()) != EOF && c != '\n' && isspace(c))
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



static int
ValidateRegisterCommand(void)
{
    int c;
    int hasId = FALSE; int hasName = FALSE; int hasPurchase = FALSE;


    while( !(hasId && hasName && hasPurchase) ){ // while mark of id/name/purchase is not finish
        c = eatSpace();
        if (c == '-'){ // if this is a valid option, if not -, this is undefined option/need more option.
            c = getValid_c();
            switch (c){
                case 'i':
                    if (hasId){
                        fprintf(stderr, "%s", ERR_SAME_OPT);
                        eatLine();
                        return FALSE;
                    }else{
                        if(verify_record_Id()){ hasId = TRUE;}
                        else {return FALSE;}
                    }
                        break;

                case 'n':
                    if (hasName){
                        fprintf(stderr, "%s", ERR_SAME_OPT);
                        eatLine();
                        return FALSE;
                    }else{
                        if(verify_record_name()){ hasName = TRUE;}
                        else {return FALSE;}
                        break;
                    }
                case 'p':
                    if (hasPurchase){
                        fprintf(stderr, "%s", ERR_SAME_OPT);
                        eatLine();
                        return FALSE;
                    }else{
                        if(verify_record_purchase()){ hasPurchase = TRUE;}
                        else {return FALSE;}
                        break;
                    }

                default : {
                    fprintf(stderr, "%s", ERR_UNDEF_OPT);
                    ungetc(c,stdin);
                    eatLine();
                    return FALSE;
                }
            }
        } else { // if this is a valid option, if not -, this is undefined option / need more option.
                if ( c == '\n'){
                    fprintf(stderr, "%s", ERR_NEED_OPT);
                    return FALSE;
                } else{
                    fprintf(stderr, "%s", ERR_UNDEF_OPT);
                    eatLine();
                    return FALSE;
                }
        }


    } // while mark of id/name/purchase is not finish

    if (hasId && hasName && hasPurchase){
        if (( c = getchar()) != EOF && c != '\n' && isspace(c)){
            c = eatSpace();
        }

        if (c == '\n'){
            //printf("finish registor");
            return TRUE;
        } else {
            if (c == '-'){
                if( (c = getValid_c()) == 'p' || c == 'i' || c == 'n'){
                    ungetc(c,stdin);
                    eatLine();
                    fprintf(stderr, "%s", ERR_SAME_OPT);
                    return FALSE;
                }
            }
            ungetc(c,stdin);
            eatLine();
            fprintf(stderr, "%s", ERR_UNDEF_OPT);
            return FALSE;
        }
    }


    fprintf(stderr, "%s", ERR_UNDEF_OPT);
    ungetc(c,stdin);
    eatLine();
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
    if (isalpha(c) || isdigit(c) || c == '-' || c == '_' || c == '.'){
        return TRUE;
    }
    return FALSE;
}

//verify and record id
int verify_record_Id(){
    int input_length = 0;
    char c;

    if (( c = getchar()) != EOF && isspace(c) && c != '\n' ){ // the first character should be space
        c = eatSpace();
        while(input_length < MAX_NUM_ID_CHARS && isLegalId(c) ){
            id[reg_no][input_length] = c ;
            input_length++;
            c = getValid_c();
        }
//        if (input_length >= MAX_NUM_ID_CHARS){
//            if ( !isspace(c) ){
//                fprintf(stderr, "%s", ERR_INVALID_ARG);
//                ungetc(c, stdin);
//                eatLine();
//                return FALSE;
//            }
//        }
    } else { // the first character should be space, if not

        fprintf(stderr, "%s", ERR_UNDEF_OPT);
        ungetc(c,stdin);
        eatLine();
        return FALSE;
    }


    if (isspace(c)){ // check after id input, it should be a space/tab/\n
        ungetc(c, stdin);
        //printf("id record!\n");
        return TRUE;
    } else {
        fprintf(stderr, "%s", ERR_INVALID_ARG);
        ungetc(c, stdin);
        eatLine();
        return FALSE;
    }

}

//verify and record name
int verify_record_name(){  // right after -n
    int input_length = 0;
    char c;

    if (( c = getchar()) != EOF && isspace(c) && c != '\n'){ // the first character should be space/tab, eat it
        c = eatSpace(); // eat the rest of the space/tab

        if (c == '\'' ){ // when the first character of name is '
            c = getValid_c();
            while ( input_length < MAX_NUM_NAME_CHARS && c != '\''){ // while character is not ', which means not finish
                if (c == '\\'){
                    c = getValid_c();
                    if (isLegalName(c) || c == ' ' || c == '\''){
                        name[reg_no][input_length] = c ;
                        input_length++;
                        c = getValid_c();
                    }else{
                        fprintf(stderr, "%s", ERR_INVALID_ARG);
                        ungetc(c, stdin);
                        eatLine();
                        return FALSE;
                    }
                } else {  // normal situation , no slash
                    if (isLegalName(c) || c == ' ' ){
                        name[reg_no][input_length] = c ;
                        input_length++;
                        c = getValid_c();
                    } else{
                        fprintf(stderr, "%s", ERR_INVALID_ARG);
                        ungetc(c, stdin);
                        eatLine();
                        return FALSE;
                    }
                }

            }
            if ( input_length >= MAX_NUM_NAME_CHARS || input_length == 0){ //  exceed or no input
                if (input_length == 0){
                    fprintf(stderr, "%s", ERR_INVALID_ARG);
                    ungetc(c, stdin);
                    eatLine();
                    return FALSE;
                }
                if ( c != '\''){
                    fprintf(stderr, "%s", ERR_INVALID_ARG);
                    ungetc(c, stdin);
                    eatLine();
                    return FALSE;
                } else{
                    c = getValid_c();
                }
            } else {  // if not exceed
                if (c == '\''){
                    c = getValid_c(); // while the character is ', great
                } else {
                    fprintf(stderr, "%s", ERR_INVALID_ARG);
                    ungetc(c, stdin);
                    eatLine();
                    return FALSE;
                }
            }

        } else{ // when the first character is not '
            while(input_length < MAX_NUM_NAME_CHARS && (isLegalName(c) || c == '\\') ){
                  if (c == '\\'){
                    c = getValid_c();
                    if (isLegalName(c) || c == ' ' || c == '\''){
                        name[reg_no][input_length] = c ;
                        input_length++;
                        c = getValid_c();
                    }else{
                        fprintf(stderr, "%s", ERR_INVALID_ARG);
                        ungetc(c, stdin);
                        eatLine();
                        return FALSE;
                    }
                  } else {
                      if (isLegalName(c)){
                          name[reg_no][input_length] = c ;
                          input_length++;
                          c = getValid_c();
                      } else {
                          fprintf(stderr, "%s", ERR_INVALID_ARG);
                          ungetc(c, stdin);
                          eatLine();
                          return FALSE;
                      }
                  }
            }

        }
    } else { // the first character should be space/tab, if not

        fprintf(stderr, "%s", ERR_UNDEF_OPT);
        ungetc(c,stdin);
        eatLine();
        return FALSE;
    }




    if (isspace(c)){ // check after name input, it should be a space/tab/\n
        ungetc(c, stdin);
        //printf("name record!\n");
        return TRUE;
    } else {
        fprintf(stderr, "%s", ERR_INVALID_ARG);
        ungetc(c, stdin);
        eatLine();
        return FALSE;
    }


}


//verify and record purchase
int verify_record_purchase(){  // right after -p
    int input_length = 0;
    char c;


    if (( c = getchar()) != EOF && isspace(c) && c != '\n'){ // the first character should be space/tab, eat it
        c = eatSpace(); // eat the rest of the space/tab

        if ( c == '0' ){ // if the first digit is 0
            if ( isspace(c = getValid_c())){ // check second digit, if it finish
                ungetc(c,stdin);
                //printf("purchase record!\n");
                return TRUE;
            }
            fprintf(stderr, "%s", ERR_INVALID_ARG);
            ungetc(c,stdin);
            eatLine();
            return FALSE;
        } else {
            while(input_length < MAX_NUM_DIGITS && isLegalPurchase(c) ){
                purchase[reg_no][input_length] = c ;
                input_length++;
                c = getValid_c();
            }
            if ( !(isLegalPurchase(c)) && !isspace(c) ){
                fprintf(stderr, "%s", ERR_INVALID_ARG);
                ungetc(c, stdin);
                eatLine();
                return FALSE;
            }
        }
    } else { // the first character should be space/tab, if not
        fprintf(stderr, "%s", ERR_UNDEF_OPT);
        ungetc(c,stdin);
        eatLine();
        return FALSE;
    }

//    if (input_length >= MAX_NUM_DIGITS){
//        fprintf(stderr, "%s", ERR_INVALID_ARG);
//        ungetc(c,stdin);
//        eatLine();
//        return FALSE;
//    }



    if (isspace(c)){ // check after purchase input, it should be a space/tab/\n
        ungetc(c,stdin);
        //printf("purchase record!\n");
        return TRUE;
    } else {
        fprintf(stderr, "%s", ERR_INVALID_ARG);
        ungetc(c, stdin);
        eatLine();
        return FALSE;
    }

}

/*---------------------------------------------------------*/
/* check whether input character name is legal               */
/* Input : c (a character of name)                           */
/* Return : TRUE or FALSE                                  */
/*                                                         */
/* a legal name  */

int isLegalName(char c){
    if (isalpha(c) ||  c == '-' || c == '.' || c == '\''){
        return TRUE;
    }
    return FALSE;
}



/*---------------------------------------------------------*/
/* check whether input character purchase is legal               */
/* Input : c (a character of name)                           */
/* Return : TRUE or FALSE                                  */
/*                                                         */
/* a legal purchase  */

int isLegalPurchase(char c){
    if (isdigit(c) ){
        return TRUE;
    }
    return FALSE;
}


/*--------------------------------------------------------------------*/
/* Check whether an unreg(unregister) or a find(search) command valid.*/
/* Input: no argument                                                 */
/* Return value: TRUE(valid) or FALSE(invalid)                        */
/*--------------------------------------------------------------------*/
static int
ValidateUnregisterOrFindCommand(void)
{

    int c;
    int hasId = FALSE; int hasName = FALSE;

    c = eatSpace();
    if (c == '-'){ // if '-' this is a valid option, if not -, this is undefined option/need more option.
        c = getValid_c();
        switch (c){
            case 'i':
                if(verify_record_Id()){hasId = TRUE;}
                else{return FALSE;}
                break;
            case 'n':
                if(verify_record_name()){ hasName = TRUE;}
                else {return FALSE;}
                break;
            default : {
                fprintf(stderr, "%s", ERR_UNDEF_OPT);
                ungetc(c,stdin);
                eatLine();
                return FALSE;
            }
        }
    } else { // if this is a valid option, if not -, this is undefined option / need more option.
        if ( c == '\n'){
            fprintf(stderr, "%s", ERR_NEED_OPT);
            return FALSE;
        } else{
            fprintf(stderr, "%s", ERR_UNDEF_OPT);
            eatLine();
            return FALSE;
        }
    }

    if (hasId || hasName){
        if (( c = getchar()) != EOF && c != '\n' && isspace(c)){
            c = eatSpace();
        }

        if (c == '\n'){
            printf("finish unreg or find");
            return TRUE;
        } else {
            if (c == '-'){
                c = getValid_c();
                if( c == 'i'){
                    if(hasId){
                        ungetc(c,stdin);
                        eatLine();
                        fprintf(stderr, "%s", ERR_SAME_OPT);
                        return FALSE;
                    } else{
                        ungetc(c,stdin);
                        eatLine();
                        fprintf(stderr, "%s", ERR_AMBIG_ARG);
                        return FALSE;
                    }
                }
                if( c == 'n'){
                    if(hasName){
                        ungetc(c,stdin);
                        eatLine();
                        fprintf(stderr, "%s", ERR_SAME_OPT);
                        return FALSE;
                    } else{
                        ungetc(c,stdin);
                        eatLine();
                        fprintf(stderr, "%s", ERR_AMBIG_ARG);
                        return FALSE;
                    }
                }
            }
        }
    }


    fprintf(stderr, "%s", ERR_UNDEF_OPT);
    ungetc(c,stdin);
    eatLine();
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
