#ifndef IMS_SIMULATOR_H
#define IMS_SIMULATOR_H

#include <set>
#include <random>

// -----------------------------------------------------------------------------
// RNG

extern std::default_random_engine generator;

void set_seed(int seed);

double uniform();

double uniform(double start, double end);

double expo(double exp_arg);

double normal(double m, double s);

// -----------------------------------------------------------------------------
// Event

class Event {
public:
    double act_time{};

    unsigned int priority = 1;

    virtual void Behavior() = 0;

    void activate();

    void activate(double new_activation_time);
};

// -----------------------------------------------------------------------------
// EventComparator

class EventComparator {
public:
    bool operator() (const Event* e1, const Event* e2) const;
};

// -----------------------------------------------------------------------------
// simulation control

extern double curr_time;

extern double end_time;

extern std::multiset<Event *, EventComparator> calendar;

void init(double start_time, double end_time);

void run();


#endif //IMS_SIMULATOR_H
