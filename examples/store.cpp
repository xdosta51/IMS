#include "../simulator.h"
#include <iostream>

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
    // Store example
    init(0,1000);

    (new SE1G)->activate();

    run();

    s.output();

    return 0;
}