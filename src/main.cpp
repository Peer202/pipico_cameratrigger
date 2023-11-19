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

int pulsPerRot = 1000;
bool interLED = false;
uint slice_num;
Impulsecounter* stroboCounter;

uint16_t countercompare = 0;

void onFullRot(){
  //digitalWrite(statusLEDPin,interLED);
  stroboCounter->resetCounter();
  //interLED = !interLED;
}

/*
void on_uart_rx(){
  interrupts();
  char message[8];
  int ri = 0;
  while (uart_is_readable(UART_ID)) {
    message[ri] = uart_getc(UART_ID);
    parseCommand(message);
    ri++;
  };
  interrupts();
};

void setupSerial(){
  uart_init(UART_ID,BAUD_RATE);
  gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
  gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);
  
  int UART_IRQ = UART_ID == uart0 ? UART0_IRQ : UART1_IRQ;
  irq_set_exclusive_handler(UART_IRQ, on_uart_rx);
  irq_set_enabled(UART_IRQ, true);
  uart_set_irq_enables(UART_ID, true, false);


  uart_set_fifo_enabled(UART_ID, true);

  uart_puts(UART_ID, "Device waiting for command!\n");
};
*/
void addInterrupt(){
   // attach Interupt
  attachInterrupt(digitalPinToInterrupt(syncPin),onFullRot,RISING);
  
  //interrupts();
}


void setup() {  
  // put your setup code here, to run once:
  pinMode(LED_BUILTIN,OUTPUT);
  pinMode(syncPin,INPUT_PULLDOWN);
  pinMode(KonePin,INPUT_PULLDOWN);
  pinMode(statusLEDPin,OUTPUT);
  digitalWrite(LED_BUILTIN,HIGH);
  Serial1.begin(115200);
  
  Serial1.println("Startup");
  stroboCounter = new Impulsecounter(KtwoPin,stroboPin,600);

  addInterrupt();
  //setupSerial();
  interrupts();


}

void loop() {
  if(Serial1.available() > 0){
    interrupts();
    String command = Serial1.readStringUntil('\n');
    if(command == "TEST"){
      Serial1.println("CHECK\n");
    };
    if(command.startsWith("S")){
      // Set Commmand
      uint value = command.substring(2).toInt();
      countercompare = value;
      stroboCounter->changeCounterCompare(value);
      //Serial1.println(value);
      Serial1.println("CHECK\n");
    };
    if(command.startsWith("G")){
      // Get Command
      Serial1.println(countercompare);
    };
    interrupts();
  };

