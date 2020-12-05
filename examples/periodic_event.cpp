#include "../simulator.h"
#include <iostream>

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

int main(){
    // periodic event example
    init(0,100);

    (new OnOnes)->activate();
    (new OnTwos)->activate();
    (new OnThrees)->activate();

    run();

    return 0;
}