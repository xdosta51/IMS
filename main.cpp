#include "simulator.h"
#include <iostream>
#include <ctime>

class OnOnes : public Event {
    void Behavior() override {
        std::cout << "OnOnes running at " << curr_time << std::endl;
        this->activate(curr_time + 1);
    }
};

class OnTwos : public Event {
    void Behavior() override {
        std::cout << "OnTwos running at " << curr_time << std::endl;
        this->activate(curr_time + 2);
    }
};

class OnThrees : public Event {
    void Behavior() override {
        std::cout << "OnThrees running at " << curr_time << std::endl;
        this->activate(curr_time + 3);
    }
};

class MaybeAgain : public Event {
    void Behavior() override {
        std::cout << "MaybeAgain running at " << curr_time;

        double r = uniform();
        std::cout << " r = " << r << std::endl;
        if (r < 0.75) {
            this->activate(curr_time + 1);
        }
    }
};

int main() {
    set_seed(time(nullptr)); // seed aktualnim casem
    init(0, 18);

    /*
    (new OnOnes)->activate();
    (new OnTwos)->activate();
    (new OnThrees)->activate();
    */
    (new MaybeAgain)->activate();

    run();

    return 0;
}

