#include "simulator.h"
#include <iostream>
#include <ctime>

#define POK_N 5

Store voziky("Voziky", 50);
Store lahudky("Lahudky", 2);
Facility pokladny[POK_N];
Histogram celk_doba("Celkova doba v systemu", 0,5,20);

// -----------------------------------------------------------------------------
// Event declarations

class ZakaznikGenerator : public Event{
    void behavior() override;
};
class EnterVoziky : public Event{
    void behavior() override;
public:
    double started_at = 0;
};
class DecideWhereToGo : public Event{
    void behavior() override;
public:
    double started_at = 0;
};
class EnterLahudky : public Event{
    void behavior() override;
public:
    double started_at = 0;
};
class WorkLahudky : public Event{
    void behavior() override;
public:
    double started_at = 0;
};
class LeaveLahudky : public Event{
    void behavior() override;
public:
    double started_at = 0;
};
class GoNakup : public Event{
    void behavior() override;
public:
    double started_at = 0;
};
class SeizeBestPokladna : public Event{
    void behavior() override;
public:
    double started_at = 0;
};
class WorkPokladna : public Event{
    void behavior() override;
public:
    int pokladna_i = 0;
    double started_at = 0;
};
class ReleasePokladna : public Event{
    void behavior() override;
public:
    int pokladna_i = 0;
    double started_at = 0;
};
class LeaveVoziky : public Event{
    void behavior() override;
public:
    double started_at = 0;
};

// -----------------------------------------------------------------------------
// behavior definitions

void ZakaznikGenerator::behavior() {
    auto new_zakaznik = new EnterVoziky;
    new_zakaznik->started_at = curr_time;
    new_zakaznik->activate();
    activate(curr_time + expo(8));
}
void EnterVoziky::behavior() {
    auto after_enter = new DecideWhereToGo;
    after_enter->started_at = started_at;
    enter(&voziky, after_enter, 1);
}
void DecideWhereToGo::behavior() {
    if (uniform() < 0.3){
        auto next_event = new EnterLahudky;
        next_event->started_at = started_at;
        next_event->activate();
    } else {
        auto next_event = new GoNakup;
        next_event->started_at = started_at;
        next_event->activate();
    }
}
void EnterLahudky::behavior() {
    auto after_enter = new WorkLahudky;
    after_enter->started_at = started_at;
    enter(&lahudky, after_enter, 1);
}
void WorkLahudky::behavior() {
    auto next_event = new LeaveLahudky;
    next_event->started_at = started_at;
    next_event->activate(curr_time+std::max(0.0,normal(2,0.7)));
}
void LeaveLahudky::behavior() {
    auto after_leave = new GoNakup;
    after_leave->started_at = started_at;
    leave(&lahudky, after_leave, 1);
}
void GoNakup::behavior() {
    auto next_event = new SeizeBestPokladna;
    next_event->started_at = started_at;
    next_event->activate(curr_time+uniform(10,15));
}
void SeizeBestPokladna::behavior() {
    int best_i = 0;
    for (int i = 1 ; i < POK_N; i++){
        if (pokladny[i].q1->length() < pokladny[best_i].q1->length()){
            best_i = i;
        }
    }
    auto after_seize = new WorkPokladna;
    after_seize->pokladna_i = best_i;
    after_seize->started_at = started_at;
    seize(&(pokladny[best_i]), after_seize);
}
void WorkPokladna::behavior() {
    auto next_event = new ReleasePokladna;
    next_event->pokladna_i = pokladna_i;
    next_event->started_at = started_at;
    next_event->activate(curr_time+expo(3));
}
void ReleasePokladna::behavior() {
    auto after_release = new LeaveVoziky;
    after_release->started_at = started_at;
    release(&(pokladny[pokladna_i]), after_release);
}
void LeaveVoziky::behavior() {
    celk_doba.add(curr_time-started_at);
    leave(&voziky, nullptr, 1);
}


int main() {
    set_seed(time(nullptr));

    init(0,10000);

    (new ZakaznikGenerator)->activate();

    run();

    celk_doba.output();
    voziky.output();
    lahudky.output();
    for (int i = 0; i < POK_N; i++){
        pokladny[i].output();
    }

    return 0;
}

