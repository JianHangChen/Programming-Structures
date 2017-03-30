#include <string.h>

int main(){
    char string_1[] = "ROBOTICS", string_2[100];
    printf("%u\n",StrGetLength(string_1) );

    StrCopy(string_2,string_1);

    printf("string1 = %s\n",string_1 );
    printf("string2 = %s\n",string_2 );

    return 0;
}
