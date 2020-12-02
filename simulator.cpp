// normal_distribution
#include <iostream>
#include <string>
#include <random>

unsigned seed = 1234;

void expo() {
    
    const int nrolls=10000;  // number of experiments
    const int nstars=100;    // maximum number of stars to distribute
    const int nintervals=10; // number of intervals

    std::default_random_engine generator (seed);
    std::exponential_distribution<double> distribution(3.5);

    int p[nintervals]={};

    for (int i=0; i<nrolls; ++i) {
        double number = distribution(generator);
        if (number<1.0) ++p[int(nintervals*number)];
    }

    std::cout << "exponential_distribution (3.5):" << std::endl;
    std::cout << std::fixed; std::cout.precision(1);

    for (int i=0; i<nintervals; ++i) {
        std::cout << float(i)/nintervals << "-" << float(i+1)/nintervals << ": ";
        std::cout << std::string(p[i]*nstars/nrolls,'*') << std::endl;
    } 
}

void rovnomerne() {
    const int nrolls=100000;  // number of experiments
    const int nstars=95;     // maximum number of stars to distribute
    const int nintervals=100; // number of intervals

    std::default_random_engine generator (seed);
    std::uniform_real_distribution<double> distribution(0.0,10.0);

    int p[nintervals]={};

    for (int i=0; i<nrolls; ++i) {
        double number = distribution(generator);
        ++p[int(number)];
        
    }

    std::cout << "uniform_real_distribution (0.0,10.0):" << std::endl;
    std::cout << std::fixed; std::cout.precision(1);

    for (int i=0; i<10; ++i) {
        std::cout << float(i)/1 << "-" << float(i+1)/1 << ": ";
        std::cout << std::string(p[i]*nstars/nrolls,'*') << std::endl;
    }


}


void uniform() {
    const int nrolls=10000;  // number of experiments
    const int nstars=95;     // maximum number of stars to distribute
    const int nintervals=10; // number of intervals

    std::default_random_engine generator (seed);
    std::uniform_real_distribution<double> distribution(0.0,1.0);

    int p[nintervals]={};

    for (int i=0; i<nrolls; ++i) {
        double number = distribution(generator);
        ++p[int(nintervals*number)];
    }

    std::cout << "uniform_real_distribution (0.0,1.0):" << std::endl;
    std::cout << std::fixed; std::cout.precision(1);

    for (int i=0; i<nintervals; ++i) {
        std::cout << float(i)/nintervals << "-" << float(i+1)/nintervals << ": ";
        std::cout << std::string(p[i]*nstars/nrolls,'*') << std::endl;
    }



}


int main(int argc, char* argv[])
{

    if (argc < 2) {
        // Tell the user how to run the program
        std::cout << "gib seed" << "\n";
        /* "Usage messages" are a conventional way of telling the user
         * how to run a program if they enter the command incorrectly.
         */
        return 1;
    }
    else if (argc == 2) {
        std::cout << argv[1] << "\n\n\n";
        seed = atoi(argv[1]);
    }


    rovnomerne();

    uniform();

    expo();

    const int nrolls=10000;  // number of experiments
    const int nstars=100;    // maximum number of stars to distribute

    std::default_random_engine generator (seed);
    std::normal_distribution<double> distribution(5.0,2.0);

    int p[10]={};

    for (int i=0; i<nrolls; ++i) {
        double number = distribution(generator);
        if ((number>=0.0)&&(number<10.0)) ++p[int(number)];
    }

    std::cout << "normal_distribution (5.0,2.0):" << std::endl;

    for (int i=0; i<10; ++i) {
        std::cout << i << "-" << (i+1) << ": ";
        std::cout << std::string(p[i]*nstars/nrolls,'*') << std::endl;
    }
}