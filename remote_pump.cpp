/*
Code to cycle a remotely relay, intended to be attached to a water pump
Pump will cycle on and off based on a defined run time
Defined run time is based on the IR signal sent to the board
Setup for an Arduino Mega, using Arduino kit IR Receiver, and
a 12 volt relay controlling a submersible pump.
Arduino Model: Mega
IR Receiver Model: Generic off the shelf
Ultrasonic Sensor Model: HC-SR04
Author: Jessie Campbell
Last Edit: 10/21/22

TO DO:
-Add buzzer functionality
*/

#include <Servo.h>
#include <IRremote.h>

#define P1 // attac pin 1 Arduino to pin for relay
#define echoPin 2 // attach pin D2 Arduino to pin Echo of HC-SR04
#define trigPin 3 // attach pin D3 Arduino to pin Trig of HC-SR04

//constants
const int RecvPin = 11; //sets the ir input pin to 11
const int relay_pin = 1; //sets relay pin for pump to pin 1

//variables
long duration; // variable for the duration of sound wave travel
int distance; // variable for the distance measurement
int level; //variable for the level of the tank in cm

IRrecv irrecv(RecvPin);
decode_results results;

void setup() {
    Serial.begin(115200); // baud rate based on the model of IR reciever, tested expermentially
    irrecv.enableIRIn();
    irrecv.blink13(true);
    pinMode(relay_pin, OUTPUT); //Sets the relay pin as an OUTPUT
    pinMode(trigPin, OUTPUT); // Sets the trigPin as an OUTPUT
    pinMode(echoPin, INPUT); // Sets the echoPin as an INPUT

}

void loop() {
    if (irrecv.decode()) {
        switch (irrecv.results.value) {
        case 0xFF6897: //Keypad button "0", run pump for 5 seconds
            // if checkLevel() >= X value, do pump run, else sound buzzer (not a thing here yet);
            if (checkLevel() == 1) {
                run_pump_short();
            }
            else if (checkLevel() == 0) {
                //play sound buzzer
            }
        }
        switch (irrecv.results.value) {
        case 0xFF30CF: //Keypad button "1", run pump for one minute 
            // if checkLevel() >= X value, do pump run, else sound buzzer (not a thing here yet);
            if (checkLevel() == 1) {
                run_pump_long
            }
            else if (checkLevel() == 0) {
                //play sound buzzer
            }
        }
        irrecv.resume();
    }
}

int checkLevel() {
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    // Sets the trigPin HIGH (ACTIVE) for 10 microseconds
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    // Reads the echoPin, returns the sound wave travel time in microseconds
    duration = pulseIn(echoPin, HIGH);
    // Calculating the distance
    distance = duration * 0.034 / 2; // Speed of sound wave divided by 2 (to and from)

    if (distance <= 15.24) { //15.24 cm is 6 inches
        //return a value of 1
        level = 1;
    }
    else if (distance > 15.24) {
        //return a value of 0
        level = 0;
    }
    return level;
}

void run_pump_short() { // runs for 1 minute
    digitalWrite(relay_pin, HIGH);
    delay(5000);
    digitalWrite(relay_pin, LOW);
    delay(2000); // 2 second delay to prevent recycle of pump
}

void run_pump_long() { // runs for 1 minute
    digitalWrite(relay_pin, HIGH);
    delay(60000);
    digitalWrite(relay_pin, LOW);
    delay(2000); // 2 second delay to prevent recycle of pump
}
