#include <Arduino.h>
#include "task3.h"
#include "events.h"

static bool compareKeys(BUTTONS *pSecret, BUTTONS *pKey)
{
    bool correct = true;
    for (uint8_t i = 0; i < 7; i++)
    {
        if (pSecret[i] != pKey[i]){
            correct = false;
            break;
        }
    }

    return correct;
}

void task3()
{

    enum class TaskStates
    {
        INIT,
        WAIT_CONFIG,
        COUNTING,
        //MOF States
        SLOW,
        MEDIUM,
        FAST,
        PERMA_OFF,
        PERMA_ON
    };
    static TaskStates taskState = TaskStates::INIT;

    const uint8_t ledBlink = 35;
    const uint32_t SlowInterval = 1000U;
    const uint32_t MediumInterval = 500U;
    const uint32_t FastInterval = 250U;

    static uint32_t lasTime;
    static bool ledStatus = false;


    //Vars to delete
    // const uint8_t ledBombCounting = 26;
    // const uint8_t ledBombBoom = 25;
    // const uint32_t BOMBINTERVAL = 1000U;
    // const uint32_t LEDCOUNTERINTERVAL = 500U;
    // static uint8_t ledBombCountingState;
    // static uint32_t initBombTimer;
    // static uint32_t initLedCounterTimer;
    // static uint8_t bombCounter;


    //Hold it at the time
    static BUTTONS secret[5] = {BUTTONS::U1_BTN, BUTTONS::U1_BTN,
                                BUTTONS::U2_BTN, BUTTONS::U2_BTN,
                                BUTTONS::U1_BTN};

    static BUTTONS disarmKey[5] = {BUTTONS::NONE};
    static uint8_t keyCounter;    

    switch (taskState)
    {

    
    case TaskStates::INIT:
    {
        pinMode(ledBlink, OUTPUT);
        digitalWrite(ledBlink, LOW);
        lasTime = millis();


        // pinMode(ledBombCounting, OUTPUT);
        // pinMode(ledBombBoom, OUTPUT);
        // digitalWrite(ledBombBoom, LOW);
        // ledBombCountingState = HIGH;
        // digitalWrite(ledBombCounting, ledBombCountingState);
        // bombCounter = 20;


        keyCounter = 0;
        taskState = TaskStates::SLOW;
        break;
    }

    case TaskStates::SLOW:
    {
        if(buttonEvt.trigger == true)
        {
            buttonEvt.trigger = false;
        if(buttonEvt.whichButton == BUTTONS::U1_BTN)
        {
            taskState = TaskStates::PERMA_OFF;
            printf("Change to Perma_OFF: %d\n");
        }
        if (buttonEvt.whichButton == BUTTONS::U2_BTN)
        {
            keyCounter = 0;
            taskState = TaskStates::MEDIUM;
            printf("Change to Medium: %d\n");
        }

        }    

        break;
    }

        case TaskStates::PERMA_OFF:
    {
        if(buttonEvt.trigger == true)
        {
            buttonEvt.trigger = false;
        if(buttonEvt.whichButton == BUTTONS::U1_BTN)
        {
            taskState = TaskStates::SLOW;
            printf("Change to SLOW: %d\n");
        }
        // else if (buttonEvt.whichButton == BUTTONS::U2_BTN)
        // {
        //     keyCounter = 0;
        //     taskState = TaskStates::MEDIUM;
        //     printf("Change to Medium: %d\n");
        // }

        }    
        break;
    }



    // case TaskStates::WAIT_CONFIG:
    // {

    //     if (buttonEvt.trigger == true)
    //     {
    //         buttonEvt.trigger = false;
    //         if (buttonEvt.whichButton == BUTTONS::U1_BTN)
    //         {
    //             if (bombCounter < 60)
    //                 bombCounter++;
    //         }
    //         else if (buttonEvt.whichButton == BUTTONS::U2_BTN)
    //         {
    //             if (bombCounter > 10)
    //                 bombCounter--;
    //         }

    //         else if (buttonEvt.whichButton == BUTTONS::ARM_BTN)
    //         {
    //             initLedCounterTimer = millis();
    //             initBombTimer = millis();
    //             keyCounter = 0;
    //             taskState = TaskStates::COUNTING;
    //         }
    //         Serial.print("Counter: ");
    //         Serial.print(bombCounter);
    //         Serial.print("\n");
    //     }

    //     break;
    // }


    // case TaskStates::COUNTING:
    // {

    //     uint32_t timeNow = millis();

    //     if ((timeNow - initBombTimer) > BOMBINTERVAL)
    //     {
    //         initBombTimer = timeNow;
    //         bombCounter--;
    //         Serial.print("Counter: ");
    //         Serial.print(bombCounter);
    //         Serial.print("\n");
    //         if (bombCounter == 0)
    //         {
    //             ledBombCountingState = HIGH;
    //             Serial.print("BOMB BOOM\n");
    //             digitalWrite(ledBombBoom, HIGH);
    //             delay(2000);
    //             digitalWrite(ledBombBoom, LOW);
    //             digitalWrite(ledBombCounting, ledBombCountingState);
    //             bombCounter = 20;
    //             taskState = TaskStates::WAIT_CONFIG;
    //         }
    //     }
    //     if ((timeNow - initLedCounterTimer) > LEDCOUNTERINTERVAL)
    //     {
    //         initLedCounterTimer = timeNow;
    //         ledBombCountingState = !ledBombCountingState;
    //         digitalWrite(ledBombCounting, ledBombCountingState);
    //     }

    //     if (buttonEvt.trigger == true)
    //     {
    //         buttonEvt.trigger = false;
    //         disarmKey[keyCounter] = buttonEvt.whichButton;
    //         keyCounter++;
    //         if (keyCounter == 7)
    //         {
    //             keyCounter = 0;
    //             if (compareKeys(secret, disarmKey) == true)
    //             {
    //                 ledBombCountingState = HIGH;
    //                 digitalWrite(ledBombCounting, ledBombCountingState);
    //                 Serial.print("BOMB DISARM\n");
    //                 bombCounter = 20;
    //                 taskState = TaskStates::WAIT_CONFIG;
    //             }
    //         }
    //     }

    //     break;
    // }


    default:
    {
        break;
    }
    }
}