/*
 Name:		transmitterCode.ino
 Created:	1/5/2017 9:21:45 AM
 Author:	Cullen Quine
 */
// INCLUDE STATEMENTS
#include "Controller.h"
#include "trim.h"
#include "buttonManager.h"
#include "printf.h"
#include "consts.h"

// USER DEFINED CONSTANTS
// toggle Serial input/output
#define debug 1
// default channel number
int channel = 1;
// PID default
#define defaultP 1
#define defaultI 0
#define defaultD .5

/************************
 *** GLOBAL VARIABLES****
 ************************/
// radio
RF24 radio(CEpin, CSpin);
// package struct
rx_values_t rxValues;
// timing variables for timing loop
unsigned long curtime, prevTime;
float P = defaultP, I = defaultI, D = defaultD;
// trim variables for adjusting for drift
trim rollTrim(upRollPin, downRollPin);
trim pitchTrim(upPitchPin, downPitchPin);
trim yawTrim(upYawPin, downYawPin);
int8_t throttleCal = 0, pitchCal = 0, rollCal = 0;
// controller variable: in charge of sending/receiving true means this is for the controller (swaps addresses)
Controller controller(&radio, channel, true);
// two buttons on the joysticks for whatever you want
buttonManager flipButton(flipPin, 0x0), modeButton(modePin, 0x0);
/************************
 *****SETUP FUNCTION*****
 ************************/
//SETUP FUNCTION: Initializes Serial, controller, buttons, and radio
void setup() {
    //initializes the radio
    controller.init();
    if (debug) {
        Serial.begin(38400);
        Serial.setTimeout(10);
        printf_begin();
    }
    //buttons
    //sets pinModes for buttons/LEDs
    initButtons();
    //sets up the default PID values
    initPID();
    if (debug) {
        Serial.println("printing radio details");
        radio.printDetails();
    }
    prevTime = millis();
}
//LOOP: Main function of the Controller
void loop() {
    if (debug)
        // check Serial for user input
        updateSerial();
    // update the current battery voltage of the controller
    updateBattery();
    // read in the values from the joysticks
    updateJoysticks();
    // check to see if a trim button was pressed and update the trim value
    updateTrims();
    // send the struct to the quadcopter
    //    Serial.print(" :\t"); Serial.print(rxValues.throttle);
    //    Serial.print("\t"); Serial.print(rxValues.yaw);
    //    Serial.print("\t"); Serial.print(rxValues.pitch);
    //    Serial.print("\t"); Serial.print(rxValues.roll);
    //    Serial.print("\t"); Serial.print(rxValues.flip);
    //    Serial.print("\t"); Serial.print(rxValues.highspeed);
    //    Serial.print("\t"); Serial.print(rxValues.P);
    //    Serial.print("\t"); Serial.print(rxValues.I);
    //    Serial.print("\t"); Serial.println(rxValues.D);
    controller.send(&rxValues);
    // receive a struct from the quadcopter (if available)
    if (controller.receive(&rxValues)) {
        // update the quadcopter light
        auxIndicator(rxValues.auxLED);
    }
    curtime = millis();
    Serial.print("loop time: ");
    Serial.println(curtime - prevTime);
    prevTime = curtime;
}

I DIDN’T CHECK THE DIFFERENCES i just added the radio info
#include "Controller.h"
#include "printf.h"

//THIS IS YOUR TEAM NUMBER

#define channel 1
#define PALevel RF24_PA_HIGH
#define CE A0
#define CS A1
// test led (should be pwm)
#define led 3
/********************************
 **** Voltage Divider Consts ****
 ********************************/

#define contBattPin A2
#define R4 1500
#define R3 1000
#define logicVolt 3.3
#define R4 1500
#define R3 1000
#define logicVolt 3.3
#include <nRF24L01.h>
#include <RF24.h>
#include <RF24_config.h>

RF24 radio(CE, CS);
rx_values_t rxValues;
// set up controller: pass it radio, channel #, and false since it is not the controller
Controller controller(&radio, channel, false);

char pipes[2] = {'l', 's'};
void startListeningPrep(uint8_t sizes) {
    radio.setPayloadSize(sizes);
    radio.setChannel(channel);
    radio.setCRCLength(RF24_CRC_16);
    radio.setDataRate(RF24_1MBPS);
    rx_values_t rxValues;
}
void setup() {
    Serial.begin(38400);
    // initalize the radio
    radio.begin();
    printf_begin();
    radio.setPALevel(RF24_PA_HIGH);
    startListeningPrep(32);
    radio.openReadingPipe(1, pipes[1]);
    radio.openWritingPipe(pipes[0]);
    radio.startListening();
    
    controller.init();
    pinMode(led, OUTPUT);
}
void loop() {
    if (!controller.isFunctioning()) {
        Serial.println("EMERGENCY!! TURN OFF ALL MOTORS AND STOP RUNING CODE");
        return;
    }
    //only print values if new values have been received
    //controler.receive will return however many values were in the buffer
    if (controller.receive(&rxValues))
    {
        analogWrite(led, rxValues.throttle);
        Serial.print(" :\t"); Serial.print(rxValues.throttle);
        Serial.print("\t"); Serial.print(rxValues.yaw);
        Serial.print("\t"); Serial.print(rxValues.pitch);
        Serial.print("\t"); Serial.print(rxValues.roll);
        Serial.print("\t"); Serial.print(rxValues.flip);
        Serial.print("\t"); Serial.print(rxValues.highspeed);
        Serial.print("\t"); Serial.print(rxValues.P);
        Serial.print("\t"); Serial.print(rxValues.I);
        Serial.print("\t"); Serial.println(rxValues.D);
    }
    updateBattery();
    // send values for led back to the controller
    controller.send(&rxValues);
}
void updateBattery() {
    int batRead = analogRead(contBattPin);
    //calc bat voltage (mV)
    unsigned long batVolt = (batRead * logicVolt * (R3 + R4)) / (R4) * 1000 / 1023;
    if (batVolt < 7400) {
        // tell the controller to turn on the led
        rxValues.auxLED = true;
    }
}


