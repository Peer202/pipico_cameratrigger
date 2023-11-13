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
#include "counter.h"

int syncPin = 21; // KO / Once per Rotation 
int KonePin = 20; // K2+ / 1000 per rot
int KtwoPin = 19; // K1+ / 1000 per rot

int triggerPin = 21; // Trigger Output
int statusLEDPin = 11;

int pulsPerRot = 1000;
int intCount = 0;
bool interLED = false;
unsigned long millisLastRot = 0;
float currentRPS = 0;
uint slice_num;
Impulsecounter *Ktwo_counter;

void onFullRot(){
  digitalWrite(LED_BUILTIN,interLED);

  uint16_t intCountPWM = Ktwo_counter->getCounter();
  Ktwo_counter->resetCounter();
  Serial1.println("intCount = " + String(intCount));
  Serial1.println("intCountPWM = " + String(intCountPWM));
  //currentRPS = 1000 / (( (float) millis()) - millisLastRot);
  //Serial1.println("rps = " + String(currentRPS));
  //millisLastRot = millis();
  //delay(10);
  if(intCountPWM == pulsPerRot){
    digitalWrite(statusLEDPin,HIGH);
  }
  else{
    digitalWrite(statusLEDPin,LOW);
  }
  interLED = !interLED;
  intCount = 0;
}
void onIncrement(){ 
  intCount++;
}

void onOverflowInterrupt(){}

void addInterrupt(){
   // attach Interupt
  attachInterrupt(digitalPinToInterrupt(syncPin),onFullRot,RISING);
  //attachInterrupt(digitalPinToInterrupt(KonePin),onIncrement,RISING);
  interrupts();
}


void setup() {
  // put your setup code here, to run once:
  pinMode(LED_BUILTIN,OUTPUT);
  pinMode(syncPin,INPUT_PULLDOWN);
  pinMode(KonePin,INPUT_PULLDOWN);
  //pinMode(triggerPin,OUTPUT);
  pinMode(statusLEDPin,OUTPUT);

  Serial1.begin(115200);
  Ktwo_counter = new Impulsecounter(KtwoPin,200);
  addInterrupt();

}

void loop() {
  // put your main code here, to run repeatedly:
  /*
  digitalWrite(LED_BUILTIN,HIGH);
  delay(500);
  digitalWrite(LED_BUILTIN,LOW);
  delay(500);
  */
}
