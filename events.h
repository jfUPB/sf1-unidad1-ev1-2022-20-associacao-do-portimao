#ifndef _EVENTS_H_
#define _EVENTS_H_

enum class BUTTONS{NONE, U1_BTN,U2_BTN};

typedef struct{
    bool trigger;
    BUTTONS whichButton;
} buttonEvt_t;

extern buttonEvt_t buttonEvt;

#endif