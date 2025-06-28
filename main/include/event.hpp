#ifndef EVENT_HPP 
#define EVENT_HPP

#include <iostream>

struct Event {
    int orderID;

    explicit Event(int id): orderID(id) {};
};

#ifndef NO_LOGGING
// Formats the event log.
std::ostream& operator<<(std::ostream &out, const Event& event) {
    out << "Event Log: ";
    out << "orderID: " << event.orderID;
    return out;
}
#endif

#endif // EVENT_HPP
