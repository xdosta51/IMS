// normal_distribution
#include <iostream>
#include <string>
#include <random>

unsigned seed = 1234;
std::default_random_engine generator;

double expo(double exp_arg) {
    
    const int nrolls=10000;  // number of experiments
    const int nstars=100;    // maximum number of stars to distribute
    const int nintervals=10; // number of intervals

    double return_number;
    
    std::exponential_distribution<double> distribution(exp_arg);

    int p[nintervals]={};

    for (int i=0; i<nrolls; ++i) {
        double number = distribution(generator);
        return_number = number;
        if (number<1.0) ++p[int(nintervals*number)];
    }

    std::cout << "exponential_distribution " << exp_arg << ":" << std::endl;
    std::cout << std::fixed; std::cout.precision(1);

    for (int i=0; i<nintervals; ++i) {
        std::cout << float(i)/nintervals << "-" << float(i+1)/nintervals << ": ";
        std::cout << std::string(p[i]*nstars/nrolls,'*') << std::endl;
    } 

    return return_number;
}

double rovnomerne(double start, double end) {
    const int nrolls=100000;  // number of experiments
    const int nstars=95;     // maximum number of stars to distribute
    const int nintervals=100; // number of intervals

    double return_number;
    
    std::uniform_real_distribution<double> distribution(start,end);

    int p[nintervals]={};

    for (int i=0; i<nrolls; ++i) {
        double number = distribution(generator);
        return_number = number;
        ++p[int(number)];
        
    }

    std::cout << "uniform_real_distribution (" << start << ", " << end << ")" << std::endl;
    std::cout << std::fixed; std::cout.precision(1);

    for (int i=0; i<10; ++i) {
        std::cout << float(i)/1 << "-" << float(i+1)/1 << ": ";
        std::cout << std::string(p[i]*nstars/nrolls,'*') << std::endl;
    }
    return return_number;

}


double uniform() {
    const int nrolls=10000;  // number of experiments
    const int nstars=95;     // maximum number of stars to distribute
    const int nintervals=10; // number of intervals

    double return_number;
   
    std::uniform_real_distribution<double> distribution(0.0,1.0);

    int p[nintervals]={};

    for (int i=0; i<nrolls; ++i) {
        double number = distribution(generator);
        return_number = number;
        ++p[int(nintervals*number)];
    }

    std::cout << "uniform_real_distribution (0.0,1.0):" << std::endl;
    std::cout << std::fixed; std::cout.precision(1);

    for (int i=0; i<nintervals; ++i) {
        std::cout << float(i)/nintervals << "-" << float(i+1)/nintervals << ": ";
        std::cout << std::string(p[i]*nstars/nrolls,'*') << std::endl;
    }

    return return_number;

}

void setseed(int seed) {
    generator.seed(seed);
}

double normal(double m, double s) {
    const int nrolls=10000;  // number of experiments
    const int nstars=100;    // maximum number of stars to distribute
    double return_number;
    
    std::normal_distribution<double> distribution(m,s);
    
    int p[10]={};

    for (int i=0; i<nrolls; ++i) {
        double number = distribution(generator);
        return_number = number;
        if ((number>=0.0)&&(number<10.0)) ++p[int(number)];
    }

    std::cout << "normal_distribution (" << m << ", " << s << ")" << std::endl;

    for (int i=0; i<10; ++i) {
        std::cout << i << "-" << (i+1) << ": ";
        std::cout << std::string(p[i]*nstars/nrolls,'*') << std::endl;
    }

    return return_number;
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
        
        seed = atoi(argv[1]);
    }
    
    setseed(seed);

    double rovnomerne_start = 1.0;
    double rovnomerne_end = 1.0;

    double rovnomerne_return = rovnomerne(rovnomerne_start, rovnomerne_end);


    double uniform_return = uniform();

    double expo_number = 8.0;

    double expo_return = expo(expo_number);

    double normal_m = 5.0;
    double normal_s = 2.5;

    double normal_return = (normal_m, normal_s);
    
    std::cout << "uniform with start" << rovnomerne_start << "and end" << rovnomerne_end << "returned: " << rovnomerne_return << "\n";
    std::cout << "uniform with start 0.0 and end 1.0 returned: " << uniform_return << "\n";
    std::cout << "exponencial distribution with arg " << expo_number << " returned: " << expo_return << "\n"; 
    std::cout << "normal distribution with arg " << normal_m << " and " << normal_s << " returned: " << normal_return << "\n";

}