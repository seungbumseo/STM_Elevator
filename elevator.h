#ifndef ELEVATOR_H
#define ELEVATOR_H

typedef enum {
    FLOOR1,
    FLOOR2,
    FLOOR3,
    FLOOR4,
    STOP
} ElevatorState;

extern volatile int direction;

#endif
