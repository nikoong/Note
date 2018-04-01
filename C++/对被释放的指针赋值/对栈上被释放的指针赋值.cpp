#include<stdio.h>
#include <stdlib.h>

int *lvret(void) {
    int ret = 0;
    return ret;
}

int main(void) {
    
    int *p = lvret();
    *p = 2;
    printf("%d\n",*p);
}    


//输出2
