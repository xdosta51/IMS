all:
	g++ -std=c++17 main.cpp simulator.cpp -o samoobsluha
run: all
	./samoobsluha
event_priority:
	g++ -std=c++17 examples/event_priority.cpp simulator.cpp -o event_priority
	./event_priority
facility:
	g++ -std=c++17 examples/facility.cpp simulator.cpp -o facility
	./facility
periodic_event:
	g++ -std=c++17 examples/periodic_event.cpp simulator.cpp -o periodic_event
	./periodic_event
queue_with_priority:
	g++ -std=c++17 examples/queue_with_priority.cpp simulator.cpp -o queue_with_priority
	./queue_with_priority
rng_histogram:
	g++ -std=c++17 examples/rng_histogram.cpp simulator.cpp -o rng_histogram
	./rng_histogram
store:
	g++ -std=c++17 examples/store.cpp simulator.cpp -o store
	./store
clean:
	rm samoobsluha event_priority facility periodic_event queue_with_priority rng_histogram store
