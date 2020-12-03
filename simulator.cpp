#include "simulator.h"
#include <iostream>
#include <random>

std::default_random_engine generator;
double curr_time;
double end_time;
std::multiset<Event *, CalendarEventComparator> calendar;

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
