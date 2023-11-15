/*
 Camera Trigger Script for Versuchsaufbau
 Reads from two Inputs, reads settings from Serial ands outputs pulses when camera should be triggered

[ ] PWM Counter
  [x] Counts Inputs
  [x] Configurable Overflow
  [x] Overflow Interrupt
  [ ] Take Function Reference to for Interrupt handling

[ ] Serial
  [ ] Setup Serial Interrupt
  [ ] Pause execution of program if necessary
  [ ] Parse Delays from input
  [ ] Set Delay to Counter

[ ] Camera / Light Trigger
  [ ] Provide Function to Counter for Interrupt
  [ ] Output Signal for Camera Sync
  [ ] Test Output Signal


[ ] Testing
  [ ] Test Delays work accurate
  [ ] Test Serial Commands are read corrrectly
  [ ] Test if Camera Trigger works
*/




#include <Arduino.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"

#include "counter.h"

int syncPin = 21; // KO / Once per Rotation 
int KonePin = 20; // K2+ / 1000 per rot
int KtwoPin = 19; // K1+ / 1000 per rot

int triggerPin = 21; // Trigger Output
int stroboPin = 18;
int statusLEDPin = 11;

int pulsPerRot = 1000;
bool interLED = false;
uint slice_num;
Impulsecounter* stroboCounter;

uint16_t countercompare = 100;


void setupCounter(){
  slice_num = pwm_gpio_to_slice_num(stroboPin);

  gpio_set_function(KtwoPin,GPIO_FUNC_PWM);
  gpio_set_function(stroboPin,GPIO_FUNC_PWM);

  pwm_config cfg = pwm_get_default_config();
  pwm_config_set_clkdiv_mode(&cfg,PWM_DIV_B_RISING);
  pwm_config_set_clkdiv(&cfg,1);
  pwm_config_set_phase_correct(&cfg,false);
  pwm_config_set_wrap(&cfg,10000); // set to value the counter will never reach to prevent overwrap
  pwm_init(slice_num,&cfg,false);

  pwm_set_chan_level(slice_num, PWM_CHAN_A, 1000); // value
  pwm_set_counter(slice_num,0);

  pwm_set_enabled(slice_num,true);
  Serial1.println(String(slice_num));


};

void onFullRot(){
  digitalWrite(statusLEDPin,interLED);
  //pwm_set_counter(1,0); // reset the counter to count again
  stroboCounter->resetCounter();
  interLED = !interLED;
  countercompare = (uint16_t) countercompare + 100;
  stroboCounter->changeCounterCompare(countercompare);
}

void addInterrupt(){
   // attach Interupt
  attachInterrupt(digitalPinToInterrupt(syncPin),onFullRot,RISING);
}


void setup() {
  // put your setup code here, to run once:
  pinMode(LED_BUILTIN,OUTPUT);
  pinMode(syncPin,INPUT_PULLDOWN);
  pinMode(KonePin,INPUT_PULLDOWN);
  pinMode(statusLEDPin,OUTPUT);

  Serial1.begin(115200);
  Serial1.println("Startup");
  stroboCounter = new Impulsecounter(KtwoPin,stroboPin,300);
  addInterrupt();
  //setupCounter();

}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(LED_BUILTIN,HIGH);
  delay(100);
  digitalWrite(LED_BUILTIN,LOW);
  delay(100);
}
