#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

// Common state labels shared amongst all models.
enum StateLabel {
    // Sorted alphabetically
    DIRECTING_ORDER_TO_LINE,
    IDLE,
    INITIATING_NEW_ORDER,
    OUT_OF_ORDERS,
    PLACING_ORDER,
    STARTING_CELL_OPERATION,
    WAITING
};

const int infinity = 2147483647; // Maximum value for an integer, i.e., infinity.

#endif // CONSTANTS_HPP
