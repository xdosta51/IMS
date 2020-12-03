#include "simulator.h"
#include <iostream>
#include <ctime>

// -----------------------------------------------------------------------------
// periodic event example

class OnOnes : public Event {
    void behavior() override {
        std::cout << "OnOnes running at " << curr_time << std::endl;
        this->activate(curr_time + 1);
    }
};

class OnTwos : public Event {
    void behavior() override {
        std::cout << "OnTwos running at " << curr_time << std::endl;
        this->activate(curr_time + 2);
    }
};

class OnThrees : public Event {
    void behavior() override {
        std::cout << "OnThrees running at " << curr_time << std::endl;
        this->activate(curr_time + 3);
    }
};

// -----------------------------------------------------------------------------
// rng example

class MaybeAgain : public Event {
    void behavior() override {
        std::cout << "MaybeAgain running at " << curr_time;

        double r = uniform();
        std::cout << " r = " << r << std::endl;
        if (r < 0.75) {
            this->activate(curr_time + 1);
        }
    }
};

// -----------------------------------------------------------------------------
// event priority example

class Important : public Event {
    //unsigned int priority = 0; // implicit
    void behavior() override {
        std::cout << "Important running at " << curr_time << std::endl;
        this->activate(curr_time + 1);
    }
};

class Importanter : public Event {
public:
    explicit Importanter(unsigned int priority) : Event(priority){};
    void behavior() override {
        std::cout << "Importanter running at " << curr_time << std::endl;
        this->activate(curr_time + 2);
    }
};

// -----------------------------------------------------------------------------
// queing example

class QueueTest1 : public Event {
    void behavior() override {
        std::cout << "QT1 running at " << curr_time << std::endl;
    }
};

class QueueTest2 : public Event {
    void behavior() override {
        std::cout << "QT2 running at " << curr_time << std::endl;
    }
};

class QueueTest3 : public Event {
    void behavior() override {
        std::cout << "QT3 running at " << curr_time << std::endl;
    }
};

class QueueTestBigPrio : public Event {
public:
    explicit QueueTestBigPrio(unsigned int priority) : Event(priority){};
    void behavior() override {
        std::cout << "QTBP "<< this->priority <<" running at " << curr_time << std::endl;
    }
};

Queue test_q;

class QueuePopper : public Event {
    void behavior() override {
        if (!test_q.empty()){
            auto e = test_q.pop();
            e->activate();
            this->activate(curr_time +1);
        }

    }
};

// -----------------------------------------------------------------------------
// Facility example

class FacilityTestGenerator : public Event {
    void behavior() override;
};
class FacilityTestSeize : public Event {
    void behavior() override;
};
class FacilityTestWork : public Event {
    void behavior() override;
};
class FacilityTestRelease : public Event {
    void behavior() override;
};
class FacilityTestLeave : public Event {
    void behavior() override;
};
Facility f;

void FacilityTestGenerator::behavior() {
    (new FacilityTestSeize)->activate();
    this->activate(curr_time + (int)uniform(1,3));
    std::cout << "started new worker at " << curr_time << std::endl;
}
void FacilityTestSeize::behavior() {
    std::cout << "worker tries seize at " << curr_time << std::endl;
    seize(&f, new FacilityTestWork);
}
void FacilityTestWork::behavior() {
    std::cout << "worker starts working at " << curr_time << std::endl;
    (new FacilityTestRelease)->activate(curr_time + (int)uniform(1,10));
}
void FacilityTestRelease::behavior() {
    std::cout << "worker finished working at " << curr_time << std::endl;
    release(&f, new FacilityTestLeave);
}
void FacilityTestLeave::behavior() {
    std::cout << "worker leaves at " << curr_time << std::endl;
}

// -----------------------------------------------------------------------------
// Store example

class SE1G : public Event {
    void behavior() override;
};
class SE2E : public Event {
    void behavior() override;
};
class SE3W : public Event {
    void behavior() override;
};
class SE4L : public Event {
    void behavior() override;
};
class SE2EBIG : public Event {
    void behavior() override;
};
class SE3WBIG : public Event {
    void behavior() override;
};
class SE4LBIG : public Event {
    void behavior() override;
};
Store s(3);

void SE1G::behavior() {
    if (uniform() < 0.5) {
        (new SE2E)->activate();
        std::cout << "started new shopper at " << curr_time << std::endl;
    } else {
        (new SE2EBIG)->activate();
        std::cout << "started new shopperBIG at " << curr_time << std::endl;
    }
    this->activate(curr_time + (int)uniform(1,3));
}
void SE2E::behavior() {
    std::cout << "shoppper tries enter at " << curr_time << std::endl;
    enter(&s, new SE3W, 1);
}
void SE3W::behavior() {
    std::cout << "shoppper starts shopping at " << curr_time << std::endl;
    (new SE4L)->activate(curr_time + (int)uniform(1,10));
}
void SE4L::behavior() {
    std::cout << "shoppper leaves at " << curr_time << std::endl;
    leave(&s, nullptr, 1);
}

void SE2EBIG::behavior() {
    std::cout << "shoppperBIG tries enter at " << curr_time << std::endl;
    enter(&s, new SE3WBIG, 2);
}
void SE3WBIG::behavior() {
    std::cout << "shoppperBIG starts shopping at " << curr_time << std::endl;
    (new SE4LBIG)->activate(curr_time + (int)uniform(1,10));
}
void SE4LBIG::behavior() {
    std::cout << "shoppperBIG leaves at " << curr_time << std::endl;
    leave(&s, nullptr, 2);
}


int main() {
    set_seed(time(nullptr)); // seed aktualnim casem
    init(0, 100);

    /*
    // periodic event example
    (new OnOnes)->activate();
    (new OnTwos)->activate();
    (new OnThrees)->activate();
    */

    /*
    // rng example
    (new MaybeAgain)->activate();
    */

    /*
    // event priority example
    (new Important)->activate();
    (new Importanter(10))->activate();
    */

    /*
    // queing example
    test_q.enqueue(new QueueTest1);
    test_q.enqueue(new QueueTest2);
    test_q.enqueue(new QueueTestBigPrio(10));
    test_q.enqueue(new QueueTest3);
    test_q.enqueue(new QueueTestBigPrio(20));
    (new QueuePopper)->activate();
    */

    /*
    // Facility example
    (new FacilityTestGenerator)->activate();
    */


    /*
    // Store example
    (new SE1G)->activate();
    */


    run();

    return 0;
}

