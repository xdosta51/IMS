// normal_distribution
#include <iostream>
#include <string>
#include <random>

unsigned seed = 1234;
std::default_random_engine generator;

void setseed(int seed) {
    generator.seed(seed);
}

double expo(double exp_arg) {
    std::exponential_distribution<double> distribution(exp_arg);
    return(distribution(generator));
}

double rovnomerne(double start, double end) {
    std::uniform_real_distribution<double> distribution(start,end);
    return(distribution(generator));
}


double uniform() {
    std::uniform_real_distribution<double> distribution(0.0,1.0);
    return(distribution(generator));
}

double normal(double m, double s) {
    std::normal_distribution<double> distribution(m,s);
    return(distribution(generator));
}

int main(int argc, char* argv[])
{

    if (argc < 2) {
        std::cout << "gib seed" << "\n";
        return 1;
    }
    else if (argc == 2)
        seed = atoi(argv[1]);
    else 
        return(0);
    
    setseed(seed);

    double rovnomerne_start = 0.0;
    double rovnomerne_end = 10.0;

    double rovnomerne_return = rovnomerne(rovnomerne_start, rovnomerne_end);


    double uniform_return = uniform();

    double expo_number = 8.0;

    double expo_return = expo(expo_number);

    double normal_m = 5.0;
    double normal_s = 2.5;

    double normal_return = normal(normal_m, normal_s);
    
    std::cout << "uniform with start " << rovnomerne_start << " and end " << rovnomerne_end << " returned: " << rovnomerne_return << "\n";
    std::cout << "uniform with start 0.0 and end 1.0 returned: " << uniform_return << "\n";
    std::cout << "exponencial distribution with arg e " << expo_number << " returned: " << expo_return << "\n"; 
    std::cout << "normal distribution with arg m " << normal_m << " and arg s " << normal_s << " returned: " << normal_return << "\n";

}