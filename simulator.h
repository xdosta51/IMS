#ifndef IMS_SIMULATOR_H
#define IMS_SIMULATOR_H

#include <set>
#include <list>
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

    void enter(Store *store, Event* after_enter, unsigned int amount);

    void leave(Store *store, Event* after_leave, unsigned int amount);
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
public:
    std::multiset<QueueItem *, QueueItemComparator> q;

    unsigned int next_id;

    void enqueue(Event *e);

    void enqueue(Event *e, unsigned int amount);

    Event *pop();

    bool empty();
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
public:
    std::string name;

    Queue *q1;

    unsigned int capacity;

    explicit Store(unsigned int capacity);

    Store(std::string name, unsigned int capacity);

    Store(std::string name, unsigned int capacity, Queue *q);
};

#endif //IMS_SIMULATOR_H
