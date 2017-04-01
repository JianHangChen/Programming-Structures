#include "str.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h> /* for skeleton code */
#include <unistd.h> /* for getopt */
#include <assert.h>

#define FIND_STR        "-f"
#define REPLACE_STR     "-r"
#define DIFF_STR        "-d"

#define MAX_STR_LEN 1023

#define FALSE 0
#define TRUE  1

typedef enum {
  INVALID,
  FIND,
  REPLACE,
  DIFF
} CommandType;


/*
    The  StrNConcat()  function  appends  the pcSrc string to the pcDest string, overwriting the
       terminating null byte ('\0') at the end of pcDest, and then adds  a  terminating  null
       byte.   The  strings may not overlap, and the pcDest string must have enough space for
       the result.


    The strncat() function is similar, except that

       *  it will use at most n characters from src; and

       *  src does not need to be null-terminated if it contains n or more characters.
*/
char *StrNConcat(char *pcDest, const char *pcSrc, size_t n){
    assert(pcDest);
    assert(pcSrc);

    char *pc = pcDest;
    const char *pcSource = pcSrc;

    while(*pc != '\0'){
        pc++;
    }

    while(*pcSource != '\0' && (pcSource-pcSrc)<n){
        *pc = *pcSource;
        pcSource++;
        pc++;
    }

    *pc = '\0';

    return pcDest;
}


/*--------------------------------------------------------------------*/
/* PrintUsage()
   print out the usage of the Simple Grep Program                     */
/*--------------------------------------------------------------------*/
void
PrintUsage(const char* argv0)
{
  const static char *fmt =
    "Simple Grep (sgrep) Usage:\n"
    "%s [COMMAND] [OPTIONS]...\n"
    "\nCOMMNAD\n"
    "\tFind: -f [search-string]\n"
    "\tReplace: -r [string1] [string2]\n"
    "\tDiff: -d [file1] [file2]\n";

  printf(fmt, argv0);
}
/*-------------------------------------------------------------------*/
/* DoFind()
   Your task:
   1. Do argument validation
   - String or file argument length is no more than 1023
   - If you encounter a command-line argument that's too long,
   print out "Error: argument is too long"

   sizeof()<1023

   2. Read the each line from standard input (stdin)
   - If you encounter a line larger than 1023 bytes,
   print out "Error: input line is too long"
   - Error message should be printed out to standard error (stderr)

   3. Check & print out the line contains a given string (search-string)

   Tips:
   - fgets() is an useful function to read characters from file. Note
   that the fget() reads until newline or the end-of-file is reached.
   - fprintf(sderr, ...) should be useful for printing out error
   message to standard error

   NOTE: If there is any problem, return FALSE; if not, return TRUE  */
/*-------------------------------------------------------------------*/
int
DoFind(const char *pcSearch)
{
    char buf[MAX_STR_LEN + 2];
    int len;

    //1. Do argument validation
    if ((len = StrGetLength(pcSearch)) > MAX_STR_LEN) {
      fprintf(stderr, "Error: argument is too long");
      return FALSE;
    }

  /* Read the line by line from stdin, Note that this is an example */
    while (fgets(buf, sizeof(buf), stdin)) {

        /* check input line length */
        if ((len = StrGetLength(buf)) > MAX_STR_LEN) {
          fprintf(stderr, "Error: input line is too long\n");
          return FALSE;
        }

        //3. Check & print out the line contains a given string (search-string)
        if (StrSearch(buf,pcSearch) != NULL){
            printf("%s",buf);
        }

    }

  return TRUE;
}
/*-------------------------------------------------------------------*/
/* DoReplace()
   Your task:
   1. Do argument validation
      - String length is no more than 1023
      - If you encounter a command-line argument that's too long,
        print out "Error: argument is too long"
      - If word1 is an empty string,
        print out "Error: Can't replace an empty substring"

   2. Read the each line from standard input (stdin)
      - If you encounter a line larger than 1023 bytes,
        print out "Error: input line is too long"
      - Error message should be printed out to standard error (stderr)

   3. Replace the string and print out the replaced string

   NOTE: If there is any problem, return FALSE; if not, return TRUE  */
/*-------------------------------------------------------------------*/
int
DoReplace(const char *pcString1, const char *pcString2)
{
  char buf[MAX_STR_LEN + 2];
  char *pc_buf = buf;
  char new_string[MAX_STR_LEN + 2];
  char* pcnew_string = new_string;
  char* pcFind;
  size_t len,len1,len2;
  int ifFind;

  len1 = StrGetLength(pcString1);
  len2 = StrGetLength(pcString2);



  if(len1 > MAX_STR_LEN || len2 > MAX_STR_LEN){
      fprintf(stderr, "Error: argument is too long");
      return FALSE;
  }
  if(len1 == 0){
      fprintf(stderr, "Error: Can't replace an empty substring");
      return FALSE;
  }

  /* Read the line by line from stdin, Note that this is an example */
    while (fgets( buf, sizeof(buf), stdin )) {
        /* check input line length */
        if ((len = StrGetLength(buf)) > MAX_STR_LEN) {
            fprintf(stderr, "Error: input line is too long\n");
            return FALSE;
        }

        pc_buf = buf;
        *new_string = '\0';
        pcnew_string = new_string;
        ifFind = FALSE;

        /*      search pc_buf for pcString1, get a position pcFind
        concat string from positon pc_buf to pcFind to pcnew_string
        search pc_buf = pc_buf+len1 for pc string1, get a postion a2 */


        while(*pc_buf != '\0'){
            if(pcFind = StrSearch(pc_buf,pcString1) ){
                // concatenate the part before string1
                StrNConcat(pcnew_string,pc_buf,pcFind-pc_buf);
                pcnew_string = pcnew_string + (pcFind-pc_buf);
                // concatenate string2
                StrNConcat(pcnew_string,pcString2,len2);
                pcnew_string = pcnew_string + len2;

                pc_buf = pcFind + len1;
                ifFind = TRUE;

            } else {
                StrConcat(pcnew_string,pc_buf);
                break;
            }
        }

        if(ifFind){
            fprintf(stdout, "%s",new_string);
            //printf("%s",new_string);
        }

    }
    return TRUE;
}



// DoDiff()

int
DoDiff(const char *file1, const char *file2)
{
    char buf1[MAX_STR_LEN + 2];
    char buf2[MAX_STR_LEN + 2];
    size_t len;
    int line = 1;
//  1. Do argument validation
//     - file name length is no more than 1023
//     - If a command-line argument is too long,
//       print out "Error: arugment is too long" to stderr
    if(StrGetLength(file1) > MAX_STR_LEN || StrGetLength(file2) > MAX_STR_LEN){
      fprintf(stderr, "Error: argument is too long");
      return FALSE;
    }


//   2. Open the two files
//      - The name of files are given by two parameters
//      - If you fail to open either file, print out error messsage
//      - Error message: "Error: failed to open file [filename]\n"
//      - Error message should be printed out to stderr

    FILE *fp1, *fp2;
    fp1 = fopen(file1,"r");
    if(fp1 == NULL){
        fprintf(stderr, "Error: failed to open file %s\n",file1);
        return FALSE;
    }
    fp2 = fopen(file2,"r");
    if(fp2 == NULL){
        fprintf(stderr, "Error: failed to open file %s\n",file2);
        fclose(fp1);
        return FALSE;
    }

//   3. Read the each line from each file
//      - If you encounter a line larger than 1023 bytes,
//        print out "Error: input line [filename] is too long"
//      - Error message should be printed out to stderr

    while (1){
        int f1 = fgets( buf1, sizeof(buf1), fp1 );
        int f2 = fgets( buf2, sizeof(buf2), fp2 );

        if (f1 && f2){


            /* check input line length */

            if ((len = StrGetLength(buf1)) > MAX_STR_LEN) {
                fprintf(stderr, "Error: input line is too long\n");
                fclose(fp1);fclose(fp2);
                return FALSE;
            }

            if ((len = StrGetLength(buf2)) > MAX_STR_LEN) {
                fprintf(stderr, "Error: input line is too long\n");
                fclose(fp1);fclose(fp2);
                return FALSE;
            }

            //   4. Compare the two files (file1, file2) line by line
            //   5. Print out any different line with the following format
            //      file1@linenumber:file1's line
            //      file2@linenumber:file2's line
            if(StrCompare(buf1, buf2) != 0){
                fprintf(stdout, "%s@%d:%s",file1,line,buf1);
                fprintf(stdout, "%s@%d:%s",file2,line,buf2);
            }
            line += 1;
        } else if( (f1&&!f2) || (!f1&&f2) ){//If one of the files ends earlier than the other
            if ( f1 == 0 ){ // if file 1 end early
                fprintf(stderr, "Error: %s ends early at line %d\n",file1,line);
            } else { // if file2 end early
                fprintf(stderr, "Error: %s ends early at line %d\n",file2,line);
            }
            fclose(fp1);fclose(fp2);
            return FALSE;
        } else {
            break;
        }
    }

    fclose(fp1);fclose(fp2);
    return TRUE;

}
/*-------------------------------------------------------------------*/
/* CommandCheck()
   - Parse the command and check number of argument.
   - It returns the command type number
   - This function only checks number of argument.
   - If the unknown function is given or the number of argument is
   different from required number, this function returns FALSE.

   Note: You SHOULD check the argument rule later                    */
/*-------------------------------------------------------------------*/
int
CommandCheck(const int argc, const char *argv1)
{
  int cmdtype = INVALID;

  /* check minimum number of argument */
  if (argc < 3)
    return cmdtype;

  /* check command type */
  if (strcmp(argv1, FIND_STR) == 0) {
    if (argc != 3)
      return FALSE;
    cmdtype = FIND;
  }
  else if (strcmp(argv1, REPLACE_STR) == 0) {
    if (argc != 4)
      return FALSE;
    cmdtype = REPLACE;
  }
  else if (strcmp(argv1, DIFF_STR) == 0) {
    if (argc != 4)
      return FALSE;
    cmdtype = DIFF;
  }

  return cmdtype;
}
/*-------------------------------------------------------------------*/
int
main1(const int argc, const char *argv[])
{
  int type, ret;

  /* Do argument check and parsing */
  if (!(type = CommandCheck(argc, argv[1]))) {
    fprintf(stderr, "Error: argument parsing error\n");
    PrintUsage(argv[0]);
    return (EXIT_FAILURE);
  }

  /* Do appropriate job */
  switch (type) {
  case FIND:
    ret = DoFind(argv[2]);
    break;
  case REPLACE:
    ret = DoReplace(argv[2], argv[3]);
    break;
  case DIFF:
    ret = DoDiff(argv[2], argv[3]);
    break;
  }

  return (ret)? EXIT_SUCCESS : EXIT_FAILURE;
}


int
main(void)
{

    const char* filename1 = "google.txt";//"D:\\My Sources\\Desktop\\EE209 structure programing\\HW\\HW2\\google.txt";
    const char* filename2 = "microsoft.txt";//"D:\\My Sources\\Desktop\\EE209 structure programing\\HW\\HW2\\microsoft.txt";
    //char* pcString1 = "Google";
    //char* pcString2 = "Microsoft";

//    char array1[100] ="Google";
//    char array2[]="Michael";
//    char* pc;
//    pc = StrNConcat(array1,pcString2,5);
//    printf("%s\n",pc);
//    DoReplace(pcString1, pcString2);
    DoDiff(filename1,filename2);
}
