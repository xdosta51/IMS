#include "../simulator.h"
#include <iostream>
#include <ctime>

int main(){
    set_seed(time(nullptr)); // seed aktualnim casem

    // Uniform()
    Histogram unif_hist("Uniform(0,1)", 0.0,0.1,10);
    for(int i = 0; i < 10000; i++){
        unif_hist.add(uniform());
    }
    unif_hist.output();

    // Uniform(0,10)
    Histogram unif2_hist("Uniform(0,10)", 0.0,1.0,10);
    for(int i = 0; i < 10000; i++){
        unif2_hist.add(uniform(0,10));
    }
    unif2_hist.output();

    // Expo(5)
    Histogram expo_hist("Expo(2.5)", 0.0,1.0,10);
    for(int i = 0; i < 10000; i++){
        expo_hist.add(expo(2.5));
    }
    expo_hist.output();

    // Normal(5,1.5)
    Histogram normal_hist("Normal(5,1.5)", 0.0,1.0,10);
    for(int i = 0; i < 10000; i++){
        normal_hist.add(normal(5,1.5));
    }
    normal_hist.output();

    return 0;
}