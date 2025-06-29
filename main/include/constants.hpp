#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

// State phases shared amongst all models.
//
// Having Phase enums for each model, e.g., MESPhase, CellPhase, etc.,
// resulted in namespace collisions when different models shared the same phase, e.g., IDLE phase.
// Solution was to have single enum that holds all phases for all models.
enum Phase {
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
