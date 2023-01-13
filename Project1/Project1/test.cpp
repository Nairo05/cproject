#include <iostream>
#include <vector>
#include <time.h>
#include <random>

int main() {

    //srand((unsigned)time(NULL));
    // dice
    //int max = 9;
    //int min = 0;
    //for(int i=0;i<100;++i){
        //printf("%i. result: %i\n",i,
        //rand() % (max - min +1) + min
        /*Getting a random number*/
        //);
    //}

    //return 0;
    



    //Will be used to obtain a seed for the random number engine
    std::random_device rd;
    //Standard mersenne_twister_engine seeded with rd()
    std::mt19937 gen1(rd());
    std::mt19937 gen2(rd());
    std::uniform_int_distribution<> dis(1, 6);
    // role the dice 10 times
    for (int n = 0; n < 150; ++n)
        //Use dis to transform the random unsigned int generated
        //by gen into an int in [1, 6]
        std::cout << dis(gen1) << ' ' << dis(gen2) << ' ';
    std::cout << '\n';
}
