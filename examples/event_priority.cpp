#include "../simulator.h"
#include <iostream>

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
    explicit Importanter(unsigned int priority) : Event(priority) {};

    void behavior() override {
        std::cout << "Importanter running at " << curr_time << std::endl;
        this->activate(curr_time + 2);
    }
};

int main() {
    // event priority example
    init(0,20);

    (new Importanter(10))->activate();

    run();

    return 0;
}