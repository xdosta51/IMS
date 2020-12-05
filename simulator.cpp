#include "simulator.h"
#include <iostream>
#include <random>
#include <climits>

static unsigned long ix = 1;
double curr_time;
double start_time;
double end_time;
std::multiset<Event *, CalendarEventComparator> calendar;

// -----------------------------------------------------------------------------
// RNG

void set_seed(int seed) {
    ix = seed;
}

double uniform() {
    ix = ix * 69069L + 1;
    return ix / ((double) ULONG_MAX + 1);
}

double uniform(double start, double end) {
    double i = start + (end - start) * uniform();
    return (i);
}

double expo(double exp_arg) {
    return -exp_arg * log(uniform());
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

Event::Event() {
    priority = 0;
}

Event::Event(unsigned int priority) {
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
    facility->inc_requests();
    if (facility->busy()) {
        facility->q1->enqueue(after_seize);
    } else {
        facility->available = false;
        facility->availability_changed();
        after_seize->activate();
    }
}

void Event::release(Facility *facility, Event *after_release) {
    if (facility->q1->empty()) {
        facility->available = true;
        facility->availability_changed();
    } else {
        auto e = facility->q1->pop();
        e->activate();
    }

    if (after_release != nullptr) {
        after_release->activate();
    }
}

void Event::enter(Store *store, Event *after_enter, unsigned int amount) {
    if (amount > store->capacity) {
        store->q1->enqueue(after_enter, amount);
    } else {
        store->capacity -= amount;
        after_enter->activate();
        store->inc_entries();
        store->capacity_changed();
    }
}

void Event::leave(Store *store, Event *after_leave, unsigned int amount) {
    store->capacity += amount;
    store->capacity_changed();
    if (!store->q1->empty()) {
        for (auto itr = store->q1->q.begin(); itr != store->q1->q.end(); itr++) {
            if ((*itr)->amount <= store->capacity) {
                store->capacity -= (*itr)->amount;
                (*itr)->e->activate();
                store->q1->q.erase(itr);
                store->q1->add_to_stats(curr_time - (*itr)->entered_at);
                store->q1->length_changed();
                store->inc_entries();
                store->capacity_changed();
                break;
            }
        }
    }

    if (after_leave != nullptr) {
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
    start_time = _start_time;
    curr_time = _start_time;
    end_time = _end_time;
}

void run() {
    while (!calendar.empty()) {
        auto e = calendar.begin();
        calendar.erase(e);

        curr_time = (*e)->act_time;
        if (end_time < curr_time) {
            curr_time = end_time;
            std::cout << "End of simulation" << std::endl;
            return;
        }

        (*e)->behavior();
    }
    curr_time = end_time;
}

// -----------------------------------------------------------------------------
// QueueItemComparator

bool QueueItemComparator::operator()(const QueueItem *qi1, const QueueItem *qi2) const {
    if (qi1->e->priority == qi2->e->priority) {
        return qi1->id < qi2->id;
    }

    return qi1->e->priority > qi2->e->priority;
}

// -----------------------------------------------------------------------------
// Queue

void Queue::add_to_stats(double qi_time_spent) {
    if (qi_time_spent < min_time_spent) {
        min_time_spent = qi_time_spent;
    }
    if (qi_time_spent > max_time_spent) {
        max_time_spent = qi_time_spent;
    }
    time_spent.push_back(qi_time_spent);
}

void Queue::enqueue(Event *e) {
    auto qi = new QueueItem;
    qi->id = (next_id)++;
    qi->e = e;
    qi->entered_at = curr_time;
    q.insert(qi);
    length_changed();
}

void Queue::enqueue(Event *e, unsigned int amount) {
    auto qi = new QueueItem;
    qi->id = (next_id)++;
    qi->e = e;
    qi->amount = amount;
    qi->entered_at = curr_time;
    q.insert(qi);
    length_changed();
}

Event *Queue::pop() {
    auto qi = q.begin();
    q.erase(qi);
    add_to_stats(curr_time - (*qi)->entered_at);
    length_changed();
    return (*qi)->e;
}

bool Queue::empty() {
    return q.empty();
}


unsigned int Queue::length() {
    return q.size();
}

void Queue::length_changed() {
    if (q.size() > max_len) {
        max_len = q.size();
    }

    length_history.emplace_back(curr_time, q.size());

}

double Queue::avg_len() {
    std::pair<double, unsigned int> prev(0.0, 0);
    double sum = 0;

    for (auto itt = length_history.begin(); itt != length_history.end(); itt++) {
        sum += prev.second * ((*itt).first - prev.first);
        prev = (*itt);
    }
    sum += prev.second * (curr_time - prev.first);
    return sum / (curr_time - start_time);
}

void Queue::output() {
    double avg_time = std::accumulate(time_spent.begin(), time_spent.end(), 0.0) / time_spent.size();
    std::cout << "Max length=" << max_len << std::endl
              << "Avg length=" << avg_len() << std::endl
              << "Min time=" << min_time_spent << std::endl
              << "Max time=" << max_time_spent << std::endl
              << "Avg time=" << avg_time << std::endl << std::endl;
}


// -----------------------------------------------------------------------------
// Facility

Facility::Facility() {
    name = "NO NAME";
    q1 = new Queue;
}

Facility::Facility(std::string name) {
    this->name = name;
    q1 = new Queue;
}

Facility::Facility(std::string name, Queue *q) {
    this->name = name;
    q1 = q;
}

bool Facility::busy() {
    return !available;
}

void Facility::inc_requests() {
    requests += 1;
}

void Facility::availability_changed() {
    history.push_back(curr_time);
}

double Facility::utilization() {
    double time_utilized = 0;
    double prev = start_time;
    unsigned int u = 0;

    for (auto itt = history.begin(); itt != history.end(); itt++) {
        time_utilized += u * (*itt - prev);
        u = 1 - u;
        prev = *itt;
    }
    time_utilized += u * (curr_time - prev);
    return time_utilized / (curr_time - start_time);
}

void Facility::output() {
    for (int i = 0; i < 80; i++) std::cout << "*";
    std::cout << std::endl << "FACILITY " << name << std::endl;
    for (int i = 0; i < 80; i++) std::cout << "*";
    std::cout << std::endl << "Requests=" << requests << std::endl
              << "Utilization=" << utilization() << std::endl;
    for (int i = 0; i < 80; i++) std::cout << "*";
    std::cout << std::endl << ".Q1" << std::endl;
    for (int i = 0; i < 80; i++) std::cout << "*";
    std::cout << std::endl;
    q1->output();
}



// -----------------------------------------------------------------------------
// Store

Store::Store(unsigned int capacity) {
    name = "NO NAME";
    q1 = new Queue;
    this->capacity = capacity;
    initial_capacity = capacity;
    min_used = 0;
    max_used = 0;
}

Store::Store(std::string name, unsigned int capacity) {
    this->name = name;
    q1 = new Queue;
    this->capacity = capacity;
    initial_capacity = capacity;
    min_used = 0;
    max_used = 0;
}

Store::Store(std::string name, unsigned int capacity, Queue *q) {
    this->name = name;
    q1 = q;
    this->capacity = capacity;
    initial_capacity = capacity;
    min_used = 0;
    max_used = 0;
}

void Store::inc_entries() {
    entries++;
}

void Store::capacity_changed() {
    unsigned int used = initial_capacity - capacity;
    if (used < min_used) {
        min_used = used;
    }
    if (used > max_used) {
        max_used = used;
    }
    used_history.emplace_back(curr_time, used);
}

double Store::avg_used() {
    std::pair<double, unsigned int> prev(0.0, 0);
    double sum = 0;

    for (auto itt = used_history.begin(); itt != used_history.end(); itt++) {
        sum += prev.second * ((*itt).first - prev.first);
        prev = (*itt);
    }
    sum += prev.second * (curr_time - prev.first);
    return sum / (curr_time - start_time);
}

void Store::output() {
    for (int i = 0; i < 80; i++) std::cout << "*";
    std::cout << std::endl << "STORE " << name << std::endl;
    for (int i = 0; i < 80; i++) std::cout << "*";
    std::cout << std::endl;
    std::cout << "Capacity=" << initial_capacity << std::endl
              << "Enter operations=" << entries << std::endl
              << "Min used capacity=" << min_used << std::endl
              << "Max used capacity=" << max_used << std::endl
              << "Avg used capacity=" << avg_used() << std::endl;
    for (int i = 0; i < 80; i++) std::cout << "*";
    std::cout << std::endl << ".Q1" << std::endl;
    for (int i = 0; i < 80; i++) std::cout << "*";
    std::cout << std::endl;
    q1->output();
}

// -----------------------------------------------------------------------------
// Histogram

Histogram::Histogram(std::string name, double start, double step, unsigned int bins) {
    this->name = name;
    this->start = start;
    this->step = step;
    this->bins = bins;
}

void Histogram::add(double value) {
    values.push_back(value);
}

void Histogram::output() {
    std::vector<double> hist(bins, 0.0);
    values.sort();
    int b = 0;

    for (auto v = values.begin(); v != values.end(); v++) {
        if (*v < start) {
            continue;
        }
        while (*v >= start + (b + 1) * step) {
            b++;
        }
        if (b >= bins) {
            break;
        }
        hist[b] += 1;
    }

    for (int i = 0; i < 80; i++) std::cout << "=";
    std::cout << std::endl << "HISTOGRAM " << name << std::endl;
    for (int i = 0; i < 80; i++) std::cout << "=";
    std::cout << std::endl << "from|\tto\t|\tn\t|\trel" << std::endl;
    for (int i = 0; i < bins; i++) {
        std::cout << start + i * step << "\t|\t"
                  << start + (i + 1) * step << "\t|\t"
                  << hist[i] << "\t|\t"
                  << hist[i] / values.size() << std::endl;
    }
    std::cout << std::endl;
}

