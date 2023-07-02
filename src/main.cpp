#include <Arduino.h>
#include <TMC2208Stepper.h>
#include <SimpleCLI.h>

#define LED_PIN 13
#define DIR_PIN 25
// Define pins
#define EN_PIN    23 // LOW: Driver e5nabled. HIGH: Driver disabled
#define STEP_PIN  13 // Step on rising edge
TMC2208Stepper driver = TMC2208Stepper(&Serial1);  // Create driver and use

#define LINE_BUFF_LEN 30
#define MAX_ARG_LEN 10 // maximum length in chars of a given arg
#define MAX_ARG_CNT 4  // maximum number of characters


void setup(){
    pinMode(EN_PIN, OUTPUT);
    digitalWrite(EN_PIN, LOW);    // Enable driver in hardware
    Serial.begin(9600);
    Serial.println("Start...");
    // Prepare pins
    pinMode(STEP_PIN, OUTPUT);
    pinMode(LED_PIN, OUTPUT);
    pinMode(DIR_PIN, OUTPUT);
    digitalWrite(DIR_PIN, HIGH);    // Enable driver in hardware
    auto cli = CLI(&Serial);

}


bool mode = false;
void loop(){
    mode = !mode;
    delayMicroseconds(80);
    digitalWrite(STEP_PIN, !digitalRead(STEP_PIN));
    digitalWrite(LED_PIN, mode);
}


