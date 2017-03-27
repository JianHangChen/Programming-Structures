
#include <str.h>


/*NAME
       strlen - calculate the length of a string

SYNOPSIS
       #include <string.h>

       size_t strlen(const char *s);

DESCRIPTION
       The strlen() function calculates the length of the string s, excluding the terminating
       null byte ('\0').

RETURN VALUE
       The strlen() function returns the number of characters in s.

CONFORMING TO
       SVr4, 4.3BSD, C89, C99.

SEE ALSO
       string(3), strnlen(3), wcslen(3), wcsnlen(3)

COLOPHON
       This page is part of release 3.35 of the Linux man-pages project.   A  description  of
       the   project,   and   information   about   reporting   bugs,   can   be   found   at
       http://man7.org/linux/man-pages/.
*/

size_t StrGetLength(const char* pcSrc){ /* Use pointer notation */
   const char *pcEnd;
   assert(pcSrc); //assert(pcSrc != NULL); / NULL, 0, FALSE are identical
   pcEnd = pcSrc;
   while(*pcEnd) //while (*pcEnd != '\0') /* note that *(pcSrc + uiLength) is valid but is "NOT" acceptable as pointer notation */
      pcEnd++;
   return (size_t)(pcEnd - pcSrc);
}


size_t StrGetLength1(const char pcSrc[]) /* Use array notation */
{
   size_t uiLength = 0U;
   assert(pcSrc);  //assert(pcSrc != NULL);  / NULL, 0, FALSE are identical
   while(pcSrc[uiLength])   //while (pcSrc[uiLength] != '\0') / NULL, 0, FALSE are identical
      uiLength++;
   return uiLength;
}


/*
NAME
       strcpy, strncpy - copy a string

SYNOPSIS
       #include <string.h>

       char *strcpy(char *dest, const char *src);

       char *strncpy(char *dest, const char *src, size_t n);

DESCRIPTION
       The  strcpy()  function copies the string pointed to by src, including the terminating
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
       The strcpy() and strncpy() functions return a pointer to the destination string dest.

CONFORMING TO
       SVr4, 4.3BSD, C89, C99.

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

SEE ALSO
       bcopy(3),   memccpy(3),   memcpy(3),  memmove(3),  stpcpy(3),  stpncpy(3),  strdup(3),
       string(3), wcscpy(3), wcsncpy(3)

COLOPHON
       This page is part of release 3.35 of the Linux man-pages project.   A  description  of
       the   project,   and   information   about   reporting   bugs,   can   be   found   at
       http://man7.org/linux/man-pages/.*/

char *StrCopy(char *pcDest, const char* pcSrc){

}




/*NAME
       strcmp, strncmp - compare two strings

SYNOPSIS
       #include <string.h>

       int strcmp(const char *s1, const char *s2);

       int strncmp(const char *s1, const char *s2, size_t n);

DESCRIPTION
       The  strcmp() function compares the two strings s1 and s2.  It returns an integer less
       than, equal to, or greater than zero if s1 is found, respectively, to be less than, to
       match, or be greater than s2.

       The strncmp() function is similar, except it only compares the first (at most) n
       characters of s1 and s2.

RETURN VALUE
       The strcmp() and strncmp() functions return an integer less than, equal to, or greater
       than  zero  if  s1  (or  the first n bytes thereof) is found, respectively, to be less
       than, to match, or be greater than s2.

CONFORMING TO
       SVr4, 4.3BSD, C89, C99.

SEE ALSO
       bcmp(3), memcmp(3),  strcasecmp(3),  strcoll(3),  string(3),  strncasecmp(3),
       strverscmp(3), wcscmp(3), wcsncmp(3)

COLOPHON
       This  page  is  part of release 3.35 of the Linux man-pages project.  A description of
       the   project,   and   information   about   reporting   bugs,   can   be   found   at
       http://man7.org/linux/man-pages/.
*/
int StrCompare(const char* pcS1, const char* pcS2){

}


/*NAME
       strstr, strcasestr - locate a substring

SYNOPSIS
       #include <string.h>

       char *strstr(const char *haystack, const char *needle);

       #define _GNU_SOURCE         // See feature_test_macros(7)

       #include <string.h>

       char *strcasestr(const char *haystack, const char *needle);

DESCRIPTION
       The strstr() function finds the first occurrence of the substring needle in the string
       haystack.  The terminating null bytes ('\0') are not compared.

       The strcasestr() function is like strstr(), but ignores the case of both arguments.

RETURN VALUE
       These functions return a pointer to the beginning of the substring,  or  NULL  if  the
       substring is not found.

CONFORMING TO
       The  strstr()  function  conforms to C89 and C99.  The strcasestr() function is a non‐
       standard extension.

BUGS
       Early versions of Linux libc (like 4.5.26) would not allow an  empty  needle  argument
       for  strstr().   Later versions (like 4.6.27) work correctly, and return haystack when
       needle is empty.

SEE ALSO
       index(3),  memchr(3),  rindex(3),  strcasecmp(3),  strchr(3),  string(3),  strpbrk(3),
       strsep(3), strspn(3), strtok(3), wcsstr(3)

COLOPHON
       This  page  is  part of release 3.35 of the Linux man-pages project.  A description of
       the   project,   and   information   about   reporting   bugs,   can   be   found   at
       http://man7.org/linux/man-pages/.
*/
char *StrSearch(const char* pcHaystack, const char *pcNeedle){
       const char* pc;
       pc = pcHaystack;


       return (char*)pc;
}



/*
NAME
       strcat, strncat - concatenate two strings

SYNOPSIS
       #include <string.h>

       char *strcat(char *dest, const char *src);

       char *strncat(char *dest, const char *src, size_t n);

DESCRIPTION
       The  strcat()  function  appends  the src string to the dest string, overwriting the
       terminating null byte ('\0') at the end of dest, and then adds  a  terminating  null
       byte.   The  strings may not overlap, and the dest string must have enough space for
       the result.

       The strncat() function is similar, except that

       *  it will use at most n characters from src; and

       *  src does not need to be null-terminated if it contains n or more characters.

       As with strcat(), the resulting string in dest is always null-terminated.

       If src contains n or more characters, strncat() writes n+1  characters  to  dest  (n
       from  src  plus  the terminating null byte).  Therefore, the size of dest must be at
       least strlen(dest)+n+1.


RETURN VALUE
       The strcat() and strncat() functions return a pointer to the resulting string dest.

CONFORMING TO
       SVr4, 4.3BSD, C89, C99.

SEE ALSO
       bcopy(3),  memccpy(3),  memcpy(3),  strcpy(3),  string(3),  strncpy(3),   wcscat(3),
       wcsncat(3)

COLOPHON
       This  page is part of release 3.35 of the Linux man-pages project.  A description of
       the  project,  and   information   about   reporting   bugs,   can   be   found   at
       http://man7.org/linux/man-pages/.
*/
char *StrSearch(const char* pcHaystack, const char *pcNeedle);