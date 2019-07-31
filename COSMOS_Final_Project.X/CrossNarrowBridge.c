#include <stdio.h>

#include "Roach_Events.h"
#include "roach.h"
#include "Roach_Top_Level_SM.h"
#include "timers.h"
/* July 22nd, 2019
 * Met with our group and discussed possible solutions to the maze. Created two versions of state machines, a wall follower and a sweeper. 
 * July 23rd, 2019
 * Decided on coding a wall follower based off of our state machine. The code worked, but unreliably because soemtimes it would get stuck and turn in 
 * circles. 
 * July 24th, 2019
 * We attempted to fix the problem of the Roach's bumper being pressed on the first block rather than the second block.
 * We planned to fix this by setting a timer for it to reverse for half a second if a bumper was hit in the first four seconds.
 * We assume that after 4 seconds (we may change this number later) the roach will be past the first block.
 * 
 * THINGS WE NEED TO FIX:
 * - The roach succeeds it's mission unreliably, sometimes it gets stuck in the light and just keeps turning around in circles. 
 * we want to fix this problem by adding a timer restricted the amount of time it can turn.
 * -We need to fix the bumper problem, the case for reversing is incorrect, we hope to implement code where if the bumper was pressed
 * in the first two seconds then the roach reverses for half a second. We are unsure how to proceed after this, as there are too many edge 
 * cases and have decided to proceed by talking to both groups coding the first portion of the maze because this would not be a problem if
 * the roach drove straight forward into the dark instead of at an angle. 
 */

//a list of states that this SM uses:
/* This macro allows you to cleanly transition between states, and trigger
 * a STATE_TRANSITION event as well */
#define TRANSITION_TO(x) { \
    printf("Transitioning to state " #x "\r\n"); \
    current_state = x; \
    ThrowEvent(STATE_TRANSITION); }

enum {
    Idle,
    Arc_Right,
    Backward_Arc_Left,
    Reversing,
    Moving_Forward
};

int current_state;

/* This function initializes the roach state machine.
 * At a minimum, this requires setting the first state.
 * Also, execute any actions associated with initializing the SM 
 * (that is, the actions on the arrow from the black dot in the SM diagram)*/
void Initialize_CrossNarrowBridge_StateMachine(void) {
    current_state = Arc_Right;
}

int previous_HowFar_state;

/* 
 * @briefThis function feeds newly detected events to the roach state machine.
 * @param event:  The most recently detected event*/
Event Run_Roach_CrossNarrowBridge_StateMachine(Event event) {
    switch (current_state) {
        case Arc_Right:
            printf("STATE = Arc_Right\r\n");
            if (event == ENTERED_LIGHT) {
                Roach_LeftMtrSpeed(-80);
                Roach_RightMtrSpeed(30);
                TRANSITION_TO(Backward_Arc_Left);
            }
            if (event == FRONT_LEFT_BUMP_PRESSED || event == FRONT_RIGHT_BUMP_PRESSED) {
                Roach_LeftMtrSpeed(0);
                Roach_RightMtrSpeed(0);
                TRANSITION_TO(Idle);
            }

            break;

        case Backward_Arc_Left:
            printf("STATE = Backward_Arc_Left\r\n");
            if (event == ENTERED_DARK) {
                Roach_LeftMtrSpeed(80);
                Roach_RightMtrSpeed(60);
                TRANSITION_TO(Arc_Right);
            }
            if (event == FRONT_LEFT_BUMP_PRESSED || event == FRONT_RIGHT_BUMP_PRESSED) {
                current_state = Idle;
                Roach_LeftMtrSpeed(0);
                Roach_RightMtrSpeed(0);
                TRANSITION_TO(Idle);
            }
            break;
        case Idle:
            printf("STATE = Idle\r\n");
            printf("%d\r\n", Roach_LightLevel());
            Roach_LeftMtrSpeed(0);
            Roach_RightMtrSpeed(0);
            break;
        


    }
    return event;
};