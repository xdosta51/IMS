#include "simulator.h"
#include <iostream>
#include <random>
#include <climits>

static unsigned long ix = 1; 
double curr_time;
double end_time;
std::multiset<Event *, CalendarEventComparator> calendar;

// -----------------------------------------------------------------------------
// RNG

void set_seed(int seed) {
    ix = seed;
}

double uniform() {
    ix = ix * 69069L + 1; 
    return ix / ((double)ULONG_MAX + 1);
}

double uniform(double start, double end) {
    double i = start + end * uniform();
    return(i);
}

double expo(double exp_arg) {
    return - exp_arg * log(uniform());
}

double normal(double mean, double stdev) {
    double u1, u2, v1, v2, s;
    static double v2_static;
    static bool v2_is_null = true;
    
    if (v2_is_null) {
        do {
            u1 = uniform();
            u2 = uniform();

            v1 = 2 * u1 - 1;
            v2 = 2 * u2 - 1;
            s = v1 * v1 + v2 * v2;
        } while (s == 0 || s >= 1);

        v2_static = v2 * sqrt(-2 * log(s) / s);
        v2_is_null = false;
        return stdev * v1 * sqrt(-2 * log(s) / s) + mean;
    }

    v2 = v2_static;
    v2_is_null = true;
    return stdev * v2 + mean;
}

// -----------------------------------------------------------------------------
// Event

Event::Event(){
    this->priority = 0;
}

Event::Event(unsigned int priority){
    this->priority = priority;
}

void Event::activate() {
    act_time = curr_time;
    calendar.insert(this);
}

void Event::activate(double new_activation_time) {
    act_time = new_activation_time;
    calendar.insert(this);
}

void Event::seize(Facility *facility, Event *after_seize) {
    if (facility->busy()){
        facility->q1->enqueue(after_seize);
    } else {
        facility->available = false;
        after_seize->activate();
    }
}

void Event::release(Facility *facility, Event *after_release) {
    if (facility->q1->empty()){
        facility->available = true;
    } else {
        auto e = facility->q1->pop();
        e->activate();
    }

    if (after_release != nullptr) {
        after_release->activate();
    }
}

void Event::enter(Store *store, Event* after_enter, unsigned int amount){
    if (amount > store->capacity){
        store->q1->enqueue(after_enter, amount);
    } else {
        store->capacity -= amount;
        after_enter->activate();
    }
}

void Event::leave(Store *store, Event* after_leave, unsigned int amount){
    store->capacity += amount;
    if (!store->q1->empty()){
        for (auto itr = store->q1->q.begin(); itr != store->q1->q.end(); itr++){
            if ((*itr)->amount <= store->capacity){
                store->capacity -= (*itr)->amount;
                (*itr)->e->activate();
                store->q1->q.erase(itr);
                break;
            }
        }
    }

    if (after_leave != nullptr){
        after_leave->activate();
    }
}

// -----------------------------------------------------------------------------
// CalendarEventComparator

bool CalendarEventComparator::operator()(const Event *e1, const Event *e2) const {
    if (e1->act_time == e2->act_time) {
        return e1->priority > e2->priority;
    }
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

        (*e)->behavior();
    }
}

// -----------------------------------------------------------------------------
// QueueItemComparator

bool QueueItemComparator::operator()(const QueueItem *qi1, const QueueItem *qi2) const {
    if (qi1->e->priority == qi2->e->priority){
        return qi1->id < qi2->id;
    }

    return qi1->e->priority > qi2->e->priority;
}

// -----------------------------------------------------------------------------
// Queue

void Queue::enqueue(Event *e) {
    auto qi = new QueueItem;
    qi->id = (this->next_id)++;
    qi->e = e;
    this->q.insert(qi);
}

void Queue::enqueue(Event *e, unsigned int amount) {
    auto qi = new QueueItem;
    qi->id = (this->next_id)++;
    qi->e = e;
    qi->amount = amount;
    this->q.insert(qi);
}

Event * Queue::pop() {
    auto qi = this->q.begin();
    this->q.erase(qi);
    return (*qi)->e;
}

bool Queue::empty() {
    return this->q.empty();
}

// -----------------------------------------------------------------------------
// Facility

Facility::Facility(){
    this->name = "NO NAME";
    this->q1 = new Queue;
}

Facility::Facility(std::string name){
    this->name = name;
    this->q1 = new Queue;
}

Facility::Facility(std::string name, Queue* q){
    this->name = name;
    this->q1 = q;
}

bool Facility::busy() {
    return !available;
}

// -----------------------------------------------------------------------------
// Store

Store::Store(unsigned int capacity){
    this->name = "NO NAME";
    this->q1 = new Queue;
    this->capacity = capacity;
}

Store::Store(std::string name, unsigned int capacity){
    this->name = name;
    this->q1 = new Queue;
    this->capacity = capacity;
}

Store::Store(std::string name, unsigned int capacity, Queue *q){
    this->name = name;
    this->q1 = q;
    this->capacity = capacity;
}
