
#ifndef DATE_H
#define DATE_H
#endif

class Impulsecounter{
    private:
    int nCounterPin;
    int nPwnSlice;
    int counterOverflowValue;
    public:
    Impulsecounter(int pinNumber,uint16_t counterOverflowValue);
    void stopCounter();
    void resetCounter();
    uint16_t getCounter();
    void setCounterOverflow(int overflowValue);
    //static void onIrqWrap();
};