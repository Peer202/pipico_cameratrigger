#include "hardware/pwm.h"
#include "pico/stdlib.h"
#include <Arduino.h>
#include "counter.h"

static void onIrqWrap(){
    pwm_clear_irq(1); // Temporary
    Serial1.print("IRQ Overflow");
};

Impulsecounter::Impulsecounter(int pinNumber,uint16_t counterOverflowValue){
    nCounterPin = pinNumber;
    nPwnSlice = pwm_gpio_to_slice_num(nCounterPin);
    counterOverflowValue = (uint16_t) counterOverflowValue * 2;

    // Tell GPIO they are allocated to the PWM
    gpio_set_function(nCounterPin,GPIO_FUNC_PWM);
    
    pwm_config cfg = pwm_get_default_config();
    pwm_config_set_clkdiv_mode(&cfg,PWM_DIV_B_FALLING);
    pwm_config_set_clkdiv(&cfg,1);
    pwm_config_set_wrap(&cfg,counterOverflowValue);

    pwm_init(nPwnSlice,&cfg,false);
    pwm_set_enabled(nPwnSlice,true);

    // start pwm irq interrupt
    pwm_set_irq_enabled(nPwnSlice,true);
    irq_set_exclusive_handler(PWM_IRQ_WRAP,onIrqWrap);
    irq_set_enabled(PWM_IRQ_WRAP, true);

};

void Impulsecounter::stopCounter(){
    pwm_set_enabled(nPwnSlice, false);

};


void Impulsecounter::resetCounter(){
pwm_set_counter(nPwnSlice,0);
};

uint16_t Impulsecounter::getCounter(){
return int(pwm_get_counter(nPwnSlice)/2);
};

void Impulsecounter::setCounterOverflow(int overflowValue) {
    counterOverflowValue = (uint16_t) overflowValue * 2;
    pwm_set_wrap(nPwnSlice,overflowValue);
};
