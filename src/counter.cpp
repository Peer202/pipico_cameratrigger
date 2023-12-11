#include "hardware/pwm.h"
#include "pico/stdlib.h"
#include <Arduino.h>
#include "counter.h"


Impulsecounter::Impulsecounter(int triggerPinNumber,int outputPinNumber, uint16_t counterWrap){
    /*
        Constructor for the Counter Class
        Initializes the Counter, but wont start it.
    */
    this->triggerPinNumber = triggerPinNumber;
    this->outputPinNumber = outputPinNumber; 
    this->nPwmSlice = pwm_gpio_to_slice_num(this->triggerPinNumber);
    this->counterWrap = counterWrap;
    this->counterCompare = counterWrap-1;


    // Tell the GPIO Pins they are allocated to the PWM
    gpio_set_function(this->triggerPinNumber,GPIO_FUNC_PWM);
    gpio_set_function(this->outputPinNumber,GPIO_FUNC_PWM);

    pwm_config cfg = pwm_get_default_config();
    pwm_config_set_clkdiv_mode(&cfg,PWM_DIV_B_RISING);
    pwm_config_set_clkdiv(&cfg,1);
    pwm_config_set_wrap(&(cfg),(uint16_t) counterWrap); // set to value the counter will never reach to prevent overwrap
    pwm_init(this->nPwmSlice,&cfg,false);

    //pwm_set_chan_level(this->nPwmSlice, PWM_CHAN_A, this->counterCompare); // value
    pwm_set_chan_level(this->nPwmSlice, PWM_CHAN_A, (uint16_t) (counterWrap));
    pwm_set_counter(this->nPwmSlice,0);
    pwm_set_enabled(this->nPwmSlice,true);
    
};


void Impulsecounter::resetCounter(){
    /*
        Sets the Value of the Peripheral Counter to 0
    */
    pwm_set_counter(this->nPwmSlice,0);
    //Serial1.println("Counter Reset");
};

void Impulsecounter::changeCounterCompare(uint16_t newCounterCompare) {
    /*
        Used to set the Peripheral Counters Overflow Value
    */
    if(/*(newCounterCompare != this->counterCompare) && */(newCounterCompare >= 0) && (newCounterCompare < 2000)){
        pwm_set_enabled(this->nPwmSlice,false);
        this->counterCompare = newCounterCompare;
        pwm_set_chan_level(this->nPwmSlice, PWM_CHAN_A, newCounterCompare);
        pwm_set_enabled(this->nPwmSlice,true);

   };
};


void Impulsecounter::changeCounterWrap(uint16_t newWrap) {
    /*
        Used to set the Peripheral Counters Overflow Value
    */
    if((newWrap >= 0) && (newWrap <= 2000)){
        //pwm_set_enabled(this->nPwmSlice,false);
        this->counterWrap = newWrap;
        pwm_set_wrap(this->nPwmSlice, newWrap);
        pwm_set_chan_level(this->nPwmSlice, PWM_CHAN_A, newWrap);
        //pwm_set_enabled(this->nPwmSlice,true);

   };


};

void Impulsecounter::skipIncrementsforward(u_int16_t incrementsToSkip) {
    uint16_t incrementvalue = (uint16_t) this->counterWrap - incrementsToSkip;
    pwm_set_wrap(this->nPwmSlice, incrementvalue);
    pwm_set_chan_level(this->nPwmSlice, PWM_CHAN_A, incrementvalue);
    sleep_ms((1000/(2000/this->counterWrap))/2);
    //sleep_ms(50);
    pwm_set_wrap(this->nPwmSlice, this->counterWrap);
    pwm_set_chan_level(this->nPwmSlice, PWM_CHAN_A, this->counterWrap);
};
