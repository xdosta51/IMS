all:
	g++ -std=c++20 main.cpp simulator.cpp -o simulator
event_priority:
	g++ examples/event_priority.cpp simulator.cpp -o event_priority
	./event_priority
facility:
	g++ examples/facility.cpp simulator.cpp -o facility
	./facility
periodic_event:
	g++ examples/periodic_event.cpp simulator.cpp -o periodic_event
	./periodic_event
queue_with_priority:
	g++ examples/queue_with_priority.cpp simulator.cpp -o queue_with_priority
	./queue_with_priority
rng_histogram:
	g++ examples/rng_histogram.cpp simulator.cpp -o rng_histogram
	./rng_histogram
store:
	g++ examples/store.cpp simulator.cpp -o store
	./store
all tests: event_priority facility periodic_event queue_with_priority rng_histogram store
clean:
	rm simulator event_priority facility periodic_event queue_with_priority rng_histogram store
