#include "simulator.h"
#include <iostream>
#include <random>

std::default_random_engine generator;
double curr_time;
double end_time;
std::multiset<Event *, EventComparator> calendar;

// -----------------------------------------------------------------------------
// RNG

void set_seed(int seed) {
    generator.seed(seed);
}

double uniform() {
    std::uniform_real_distribution<double> distribution(0.0, 1.0);
    return distribution(generator);
}

double uniform(double start, double end) {
    std::uniform_real_distribution<double> distribution(start, end);
    return distribution(generator);
}

double expo(double exp_arg) {
    std::exponential_distribution<double> distribution(exp_arg);
    return distribution(generator);
}

double normal(double m, double s) {
    std::normal_distribution<double> distribution(m, s);
    return distribution(generator);
}

// -----------------------------------------------------------------------------
// Event

void Event::activate() {
    act_time = curr_time;
    calendar.insert(this);
}

void Event::activate(double new_activation_time) {
    act_time = new_activation_time;
    calendar.insert(this);
}

// -----------------------------------------------------------------------------
// EventComparator

bool EventComparator::operator()(const Event *e1, const Event *e2) const {
    return e1->act_time < e2->act_time;
}

// -----------------------------------------------------------------------------
// simulation control

void init(double _start_time, double _end_time) {
    curr_time = _start_time;
    end_time = _end_time;
}

void run() {
    while (!calendar.empty()) {
        auto e = calendar.begin();
        calendar.erase(e);

        curr_time = (*e)->act_time;
        if (end_time < curr_time) {
            std::cout << "End of simulation" << std::endl;
            return;
        }

        (*e)->Behavior();
    }
}


