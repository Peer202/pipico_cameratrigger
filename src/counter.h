
#include <Arduino.h>

#ifndef DATE_H
#define DATE_H
#endif

class Impulsecounter{
    private:
    int triggerPinNumber;
    int outputPinNumber;
    int nPwmSlice;
    int counterCompare;
    public:
    Impulsecounter(int triggerPinNumber,int outputPinNumber, uint16_t counterCompare);
    void resetCounter();
    void changeCounterCompare(uint16_t newCounterCompare);
};