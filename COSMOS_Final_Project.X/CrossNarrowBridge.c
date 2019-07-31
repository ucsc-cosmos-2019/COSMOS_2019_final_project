#include <stdio.h>
#include "Roach_Events.h"
#include "roach.h"
#include "Roach_Top_Level_SM.h"
#include "timers.h"
#include "serial.h"
#define timers_A 1

enum {
    Curve_Left,
    Curve_Back_Left,
    Curve_Right,
    Curve_Back_Right,
    Stop,
    Stop_Turn_Right,
    Done,
};
int current_state = Curve_Left;

void Initialize_CrossNarrowBridge_StateMachine(void) {
    Roach_RightMtrSpeed(80);
    Roach_LeftMtrSpeed(30);
    current_state = Curve_Left;
};

Event Run_Roach_CrossNarrowBridge_StateMachine(Event event) {
    switch (current_state) {
        case Curve_Left:
            if (event == FRONT_RIGHT_BUMP_PRESSED) {
                current_state = Stop;
            }
            if (event == FRONT_LEFT_BUMP_PRESSED) {
                Roach_RightMtrSpeed(0);
                Roach_LeftMtrSpeed(0);
                current_state = Done;
            }
            printf("Current state: Curve_Left\r\n");
            if (event != ENTERED_DARK) {
                current_state = Curve_Back_Left;
            }
            break;
        case Curve_Back_Left:
            if (event == FRONT_RIGHT_BUMP_PRESSED) {
                current_state = Stop;
            }
            if (event == FRONT_LEFT_BUMP_PRESSED) {
                Roach_RightMtrSpeed(0);
                Roach_LeftMtrSpeed(0);
                current_state = Done;
                printf("BUMP PRESSED\r\n");
            }
            printf("CURRENT STATE: Curve_Back_Left\r\n");
            TIMERS_InitTimer(NAV_TIMER, 350);
            Roach_RightMtrSpeed(-75);
            Roach_LeftMtrSpeed(40);
            if (event == NAV_TIMER_EXPIRED) {
                printf("TIMER EXPIREDt\r\n");
                current_state = Curve_Right;
                Roach_RightMtrSpeed(30);
                Roach_LeftMtrSpeed(80);
            }
            break;
        case Curve_Right:
            if (event == FRONT_RIGHT_BUMP_PRESSED) {
                current_state = Stop;
            }
            if (event == FRONT_LEFT_BUMP_PRESSED) {
                Roach_RightMtrSpeed(0);
                Roach_LeftMtrSpeed(0);
                current_state = Done;
                printf("BUMP PRESSED\r\n");
            }
            printf("Current state: Curve_Right\r\n");
            if (event != ENTERED_LIGHT) {
                current_state = Curve_Back_Right;
            }
            break;

        case Curve_Back_Right:
            if (event == FRONT_RIGHT_BUMP_PRESSED) {
                current_state = Stop;
            }
            if (event == FRONT_LEFT_BUMP_PRESSED) {
                Roach_RightMtrSpeed(0);
                Roach_LeftMtrSpeed(0);
                current_state = Done;
                printf("BUMP PRESSED\r\n");
            }
            printf("Current state: Curve_Back_Right\r\n");
            TIMERS_InitTimer(NAV_TIMER, 350);
            Roach_RightMtrSpeed(40);
            Roach_LeftMtrSpeed(-75);
            if (event == NAV_TIMER_EXPIRED) {
                printf("TIMER EXPIREDt\r\n");
                current_state = Curve_Left;
                Roach_RightMtrSpeed(80);
                Roach_LeftMtrSpeed(30);
            }
            break;

        case Stop:
            printf("Current state: Stop\r\n");
            TIMERS_InitTimer(NAV_TIMER, 600);
            Roach_RightMtrSpeed(-50);
            Roach_LeftMtrSpeed(-50);
            if (event == NAV_TIMER_EXPIRED) {
                current_state = Stop_Turn_Right;
            }
            break;
        case Stop_Turn_Right:
            printf("Done\r\n");
            Roach_RightMtrSpeed(40);
            Roach_LeftMtrSpeed(65);
            if (event == ENTERED_DARK) {
                printf("DONEEEEEE");
                Roach_RightMtrSpeed(0);
                Roach_LeftMtrSpeed(0);
                current_state = Done;
            }
            break;
        case Done:
            Roach_RightMtrSpeed(0);
            Roach_LeftMtrSpeed(0);
            break;

        default:
            printf("NOT A STATE, %d", current_state);
            while (1);




            return event;
            Roach_LEDSSet(Roach_LightLevel()*12 / 1023);
    }
};