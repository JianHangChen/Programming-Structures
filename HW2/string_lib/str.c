#include <assert.h> /* to use assert() */
#include <stdio.h>
#include "str.h"

/* Your task is:
   1. Rewrite the body of "Part 1" functions - remove the current
      body that simply calls the corresponding C standard library
      function.
   2. Write appropriate comment per each function
*/


/* Part 1 */
/*------------------------------------------------------------------------*/
/*NAME
       StrGetLength - calculate the length of a string

SYNOPSIS
       #include "str.h"

       size_t StrGetLength(const char* pcSrc);

DESCRIPTION
       The StrGetLength() function calculates the length of the string s, excluding the terminating
       null byte ('\0').

RETURN VALUE
       The strlen() function returns the number of characters in s.


*/

size_t StrGetLength(const char* pcSrc){ /* Use pointer notation */
   const char *pcEnd;
   assert(pcSrc); //assert(pcSrc != NULL); / NULL, 0, FALSE are identical
   pcEnd = pcSrc;
   while(*pcEnd) //while (*pcEnd != '\0') /* note that *(pcSrc + uiLength) is valid but is "NOT" acceptable as pointer notation */
      pcEnd++;
   return (size_t)(pcEnd - pcSrc);
}

/*
size_t StrGetLength1(const char pcSrc[]) // Use array notation
{
   size_t uiLength = 0U;
   assert(pcSrc);  //assert(pcSrc != NULL);  / NULL, 0, FALSE are identical
   while(pcSrc[uiLength])   //while (pcSrc[uiLength] != '\0') / NULL, 0, FALSE are identical
      uiLength++;
   return uiLength;
}
*/

/*------------------------------------------------------------------------*/

/*
NAME
       StrCopy, strncpy - copy a string

SYNOPSIS
       #include <str.h>

       char *StrCopy(char *dest, const char *src);

       char *strncpy(char *dest, const char *src, size_t n);

DESCRIPTION
       The  StrCopy()  function copies the string pointed to by src, including the terminating
       null byte ('\0'), to the buffer pointed to by dest.  The strings may not overlap,  and
       the destination string dest must be large enough to receive the copy.

       The  strncpy()  function  is  similar,  except that at most n bytes of src are copied.
       Warning: If there is no null byte among the first n bytes of src, the string placed in
       dest will not be null-terminated.

       If  the  length  of src is less than n, strncpy() pads the remainder of dest with null
       bytes.

       A simple implementation of strncpy() might be:

           char *
           strncpy(char *dest, const char *src, size_t n)
           {
               size_t i;

               for (i = 0; i < n && src[i] != '\0'; i++)
                   dest[i] = src[i];
               for ( ; i < n; i++)
                   dest[i] = '\0';

               return dest;
           }

RETURN VALUE
       The StrCopy() and strncpy() functions return a pointer to the destination string dest.

NOTES
       Some programmers consider strncpy() to be inefficient and error prone.   If  the
       programmer  knows  (i.e.,  includes code to test!)  that the size of dest is greater
       than the length of src, then strcpy() can be used.

       If there is no terminating null byte in the first n characters of src, strncpy()
       produces an unterminated string in dest.  Programmers often prevent this mistake by
       forcing termination as follows:

           strncpy(buf, str, n);
           if (n > 0)
               buf[n - 1]= '\0';

BUGS
       If the destination string of a strcpy() is not large enough, then anything might
       happen.  Overflowing fixed-length string buffers is a favorite cracker technique for
       taking complete control of the machine.  Any time a program reads or copies data
       into  a buffer,  the  program  first  needs  to  check that there's enough space.
       This may be unnecessary if you can show that overflow is impossible, but be careful:
        programs  can get changed over time, in ways that may make the impossible possible.

*/


char *StrCopy(char *pcDest, const char* pcSrc){
    size_t n = StrGetLength(pcSrc) + 1;
    size_t i;

    // check if pcSrc is empty
    // check if pcDest has enough space?!!!!
    for (i = 0; i < n ; i++)
        pcDest[i] = pcSrc[i];

    return pcDest;
//return strcpy(pcDest, pcSrc);
}


/*------------------------------------------------------------------------*/

/*NAME
       StrCompare, strncmp - compare two strings

SYNOPSIS
       #include <str.h>

       int StrCompare(const char* pcS1, const char* pcS2);


       int strncmp(const char *s1, const char *s2, size_t n);

DESCRIPTION
       The  StrCompare() function compares the two strings pcS1 and pcS2.  It returns an integer less
       than, equal to, or greater than zero if pcS1 is found, respectively, to be less than, to
       match, or be greater than pcS2.

       The strncmp() function is similar, except it only compares the first (at most) n
       characters of pcS1 and pcS2.

RETURN VALUE
       The StrCompare() and strncmp() functions return an integer less than, equal to, or greater
       than  zero  if  pcS1  (or  the first n bytes thereof) is found, respectively, to be less
       than, to match, or be greater than pcS2.

*/
int StrCompare(const char* pcS1, const char* pcS2){
    size_t n1 = StrGetLength(pcS1)+1;
    size_t n2 = StrGetLength(pcS2)+1;
    size_t i;

    for (i = 0; (i < n1) && (i < n2); i++){
        if (pcS1[i] < pcS2[i]){
            return -1;
        }
        if (pcS1[i] > pcS2[i]){
            return 1;
        }
    }

    return 0;

//  return strcmp(pcS1, pcS2);
}

/*------------------------------------------------------------------------*/
/*NAME
       StrSearch, strcasestr - locate a substring

SYNOPSIS
       #include <str.h>

       char *StrSearch(const char *haystack, const char *needle);

       char *strcasestr(const char *haystack, const char *needle);

DESCRIPTION
       The StrSearch() function finds the first occurrence of the substring needle in the string
       haystack.  The terminating null bytes ('\0') are not compared.

       The strcasestr() function is like StrSearch(), but ignores the case of both arguments.

RETURN VALUE
       These functions return a pointer to the beginning of the substring,  or  NULL  if  the
       substring is not found.*/

char *StrSearch(const char* pcHaystack, const char *pcNeedle){
    const char* pc;
    pc = pcHaystack;
    if(pc == NULL){
        return NULL;
    }
    while( *pc != '\0' ){
        int i = 0;
        for( i ; *(pcNeedle+i) != '\0' && *(pc+i) == *(pcNeedle+i) ; i++){
        }
        if (*(pcNeedle+i) == '\0'){
            return (char*)pc;
        } else{
            pc++;
        }

    }

    return NULL;


//  return strstr(pcHaystack, pcNeedle);
}

/*------------------------------------------------------------------------*/

/*
NAME
       StrConcat, strncat - concatenate two strings

SYNOPSIS
       #include <str.h>

       char *StrConcat(char *pcDest, const char* pcSrc);

       char *strncat(char *dest, const char *src, size_t n);

DESCRIPTION
       The  StrConcat()  function  appends  the pcSrc string to the pcDest string, overwriting the
       terminating null byte ('\0') at the end of pcDest, and then adds  a  terminating  null
       byte.   The  strings may not overlap, and the pcDest string must have enough space for
       the result.

       The strncat() function is similar, except that

       *  it will use at most n characters from src; and

       *  src does not need to be null-terminated if it contains n or more characters.

       As with StrConcat(), the resulting string in pcDest is always null-terminated.

       If src contains n or more characters, strncat() writes n+1  characters  to  dest  (n
       from  src  plus  the terminating null byte).  Therefore, the size of dest must be at
       least strlen(dest)+n+1.


RETURN VALUE
       The strcat() and strncat() functions return a pointer to the resulting string dest.

*/
char *StrConcat(char *pcDest, const char* pcSrc){
    assert(pcDest);
    assert(pcSrc);
    char *pc = pcDest;
    while(*pc != '\0'){
        pc++;
    }
    while(*pcSrc != '\0'){
        *pc = *pcSrc;
        pcSrc++;
        pc++;
    }

    *pc = '\0';

    return pcDest;
//  return strcat(pcDest, pcSrc);
}
