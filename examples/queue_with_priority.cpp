#include "../simulator.h"
#include <iostream>

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

int main() {
    // queing example
    init(0,20);

    test_q.enqueue(new QueueTest1);
    test_q.enqueue(new QueueTest2);
    test_q.enqueue(new QueueTestBigPrio(10));
    test_q.enqueue(new QueueTest3);
    test_q.enqueue(new QueueTestBigPrio(20));
    (new QueuePopper)->activate();

    run();

    return 0;
}