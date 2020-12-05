#ifndef IMS_SIMULATOR_H
#define IMS_SIMULATOR_H

#include <set>
#include <list>
#include <random>
#include <utility>
#include <cmath>
#include <numeric>

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

class Facility; // forward declaration
class Store; // forward declaration

class Event {
public:
    double act_time{};

    unsigned int priority = 0;

    Event();

    explicit Event(unsigned int priority);

    virtual void behavior() = 0;

    void activate();

    void activate(double new_activation_time);

    void seize(Facility *facility, Event *after_seize);

    void release(Facility *facility, Event *after_release);

    void enter(Store *store, Event *after_enter, unsigned int amount);

    void leave(Store *store, Event *after_leave, unsigned int amount);
};

// -----------------------------------------------------------------------------
// CalendarEventComparator

class CalendarEventComparator {
public:
    bool operator()(const Event *e1, const Event *e2) const;
};

// -----------------------------------------------------------------------------
// simulation control

extern double curr_time;

extern double end_time;

extern double start_time;

extern std::multiset<Event *, CalendarEventComparator> calendar;

void init(double start_time, double end_time);

void run();

// -----------------------------------------------------------------------------
// QueueItem

class QueueItem {
public:
    unsigned int id;

    Event *e;

    unsigned int amount;

    double entered_at;
};

// -----------------------------------------------------------------------------
// QueueItemComparator

class QueueItemComparator {
public:
    bool operator()(const QueueItem *e1, const QueueItem *e2) const;
};

// -----------------------------------------------------------------------------
// Queue

class Queue {
    double min_time_spent = INFINITY;

    double max_time_spent = 0;

    std::list<double> time_spent;

    std::list<std::pair<double, unsigned int>> length_history;

    unsigned int max_len = 0;

    double avg_len();

public:
    std::multiset<QueueItem *, QueueItemComparator> q;

    unsigned int next_id;

    void enqueue(Event *e);

    void enqueue(Event *e, unsigned int amount);

    Event *pop();

    bool empty();

    void length_changed();

    void add_to_stats(double qi_time_spent);

    void output();
};

// -----------------------------------------------------------------------------
// Facility

class Facility {
    unsigned int requests = 0;

    std::list<double> history;

    double utilization();

public:
    std::string name;

    Queue *q1;

    bool available = true;

    Facility();

    explicit Facility(std::string name);

    Facility(std::string name, Queue *q);

    bool busy();

    void inc_requests();

    void availability_changed();

    void output();
};

// -----------------------------------------------------------------------------
// Store

class Store {
    std::list<std::pair<double, unsigned int>> used_history;

    unsigned int initial_capacity;

    unsigned int entries = 0;

    unsigned int min_used;

    unsigned int max_used;

    double avg_used();

public:
    std::string name;

    Queue *q1;

    unsigned int capacity;

    explicit Store(unsigned int capacity);

    Store(std::string name, unsigned int capacity);

    Store(std::string name, unsigned int capacity, Queue *q);

    void inc_entries();

    void capacity_changed();

    void output();
};


// -----------------------------------------------------------------------------
// Histogram

class Histogram {
    std::list<double> values;
    std::string name;
    double start;
    double step;
    unsigned int bins;
public:
    Histogram(std::string name, double start, double step, unsigned int bins);

    void add(double value);

    void output();
};


#endif //IMS_SIMULATOR_H
