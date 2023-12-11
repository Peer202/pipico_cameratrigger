/*
 Camera Trigger Script for Versuchsaufbau
 Reads from two Inputs, reads settings from Serial ands outputs pulses when camera should be triggered

[ ] PWM Counter
  [x] Counts Inputs
  [x] Output Controll
  [x] Object function for changing counterCOmpare works

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
#include "hardware/uart.h"

#include "counter.h"
//#include "serial.h"

#define UART_ID uart0
#define BAUD_RATE 115200

#define UART_TX_PIN 0
#define UART_RX_PIN 1

int syncPin = 21; // KO / Once per Rotation 
int KonePin = 20; // K2+ / 1000 per rot
int KtwoPin = 19; // K1+ / 1000 per rot

int triggerPin = 21; // Trigger Output
int stroboPin = 18;
int statusLEDPin = 11;

int pulsPerRot = 2000;
bool interLED = false;
uint slice_num;
Impulsecounter* stroboCounter;

//uint16_t countercompare = 0;
uint16_t counterWrap = 2000;

void onFullRot(){
  stroboCounter->resetCounter();
}

void addInterrupt(){
   // attach Interupt
  attachInterrupt(digitalPinToInterrupt(syncPin),onFullRot,RISING);
  
  interrupts();
}


void setup() {  
  // put your setup code here, to run once:
  pinMode(LED_BUILTIN,OUTPUT);
  pinMode(syncPin,INPUT_PULLDOWN);
  pinMode(KonePin,INPUT_PULLDOWN);
  pinMode(statusLEDPin,OUTPUT);
  digitalWrite(LED_BUILTIN,HIGH);
  Serial.begin(115200);
  
  Serial.println("Startup");
  stroboCounter = new Impulsecounter(KtwoPin,stroboPin,counterWrap);

  //addInterrupt();

}

void loop() {
  if(Serial.available() > 0){
    //interrupts();
    String command = Serial.readStringUntil('\n');
    if(command == "TEST"){
      Serial.println("CHECK\n");
    } else if(command.startsWith("S")){
      // Set Increment Commmand
      uint value = command.substring(2).toInt();
      counterWrap = value;
      stroboCounter->changeCounterWrap(counterWrap);
      //Serial1.println(value);
      Serial.println("CHECK\n");
    } else if(command.startsWith("G")){
      // Get Command
      Serial.println(counterWrap);
    } else if(command.startsWith("T")){
      // Skip Command
      uint value = command.substring(2).toInt();
      stroboCounter->skipIncrementsforward(value);

    }else {
      Serial.println("NOCOM");
    };
    //interrupts();
  };
}

