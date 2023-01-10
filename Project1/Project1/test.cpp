#include <iostream>
#include <vector>
#include <time.h>

int main() {

    srand((unsigned)time(NULL));
    // dice
    int max = 9;
    int min = 0;
    for(int i=0;i<100;++i){
        printf("%i. result: %i\n",i,
        rand() % (max - min +1) + min
        /*Getting a random number*/
        );
    }

    return 0;
}