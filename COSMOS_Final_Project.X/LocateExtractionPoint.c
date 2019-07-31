#include <stdio.h>

#include "Roach_Events.h"
#include "roach.h"
#include "Roach_Top_Level_SM.h"
#include "timers.h"
#define ARRAY_LENGTH 8
#define MIN0 40.0
#define MAX0 50.0
#define MIN1 18.0
#define MAX1 20.0
#define MIN2 30.0
#define MAX2 33.0
//a list of states that this SM uses:

enum {
    //your states here
    Forward1,
    Forward2,
    Forward3,
    Turn90_1,
    Turn90_2,
    Turn90_3,
    Flashing
};

static int current_state;

/* This function initializes the roach state machine.
 * At a minimum, this requires setting the first state.
 * Also, execute any actions associated with initializing the SM 
 * (that is, the actions on the arrow from the black dot in the SM diagram)*/
void Initialize_LocateExtractionPoint_StateMachine(void) {
    //set current state:
    current_state = Forward1;
    Roach_LeftMtrSpeed(100);
    Roach_RightMtrSpeed(100);
    TIMERS_InitTimer(NAV_TIMER, 100);

};

/* 
 * @briefThis function feeds newly detected events to the roach state machine.
 * @param event:  The most recently detected event*/

Event Run_Roach_LocateExtractionPoint_StateMachine(Event event) {
    double distance = MeasureDistance();
    Roach_BarGraph((int) distance / 10);
    switch (current_state) {
            //SM here
        case Forward1:
            if (distance <= 5.0 && TIMERS_IsTimerExpired(NAV_TIMER)){
                current_state = Turn90_1;
                Roach_RightMtrSpeed(-100);
                Roach_LeftMtrSpeed(100);
                TIMERS_InitTimer(NAV_TIMER, 720);
            }
            break;
        case Turn90_1:
            if (event == NAV_TIMER_EXPIRED) {
                current_state = Forward2;
                Roach_RightMtrSpeed(100);
                Roach_LeftMtrSpeed(100);
            }
            break;

        case Forward2:
            if (distance >= MIN1 && distance <= MAX1) {
                Roach_RightMtrSpeed(0);
                Roach_LeftMtrSpeed(0);
                current_state = Turn90_2;
                Roach_RightMtrSpeed(-100);
                Roach_LeftMtrSpeed(100);
                TIMERS_InitTimer(NAV_TIMER, 750);
            }
            break;
        case Turn90_2:
            if (event == NAV_TIMER_EXPIRED) {
                current_state = Forward3;
                Roach_RightMtrSpeed(100);
                Roach_LeftMtrSpeed(100);
            }
            break;
        case Forward3:
            if (distance >= MIN2 && distance <= MAX2) {
                current_state = Turn90_3;
                Roach_RightMtrSpeed(-100);
                Roach_LeftMtrSpeed(100);
                TIMERS_InitTimer(NAV_TIMER, 500);
            }
            break;
        case Turn90_3:
            if (event == NAV_TIMER_EXPIRED) {
                current_state = Flashing;
                Roach_RightMtrSpeed(0);
                Roach_LeftMtrSpeed(0);
                Roach_LEDSSet(0XFFFF);
                TIMERS_InitTimer(NAV_TIMER, 100);
            }
            break;
        case Flashing:
            if (event == NAV_TIMER_EXPIRED) {
                Roach_LEDSSet(0X1234);
                TIMERS_InitTimer(NAV_TIMER, 100);
                current_state = Turn90_3;
            }
            break;
    }
    return event;
};
