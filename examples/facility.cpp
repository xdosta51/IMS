#include "../simulator.h"
#include <iostream>

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
    this->activate(curr_time + (int)uniform(1,30));
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

int main() {
    // Facility example
    init(0,1000);

    (new FacilityTestGenerator)->activate();

    run();

    f.output();

    return 0;
}