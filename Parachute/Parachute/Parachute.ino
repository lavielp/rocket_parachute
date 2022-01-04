#include <Servo.h>
// #define RLED RED_LED
// #define GLED GREEN_LED

const int REDLED = 14;
const int GREENLED = 2;
const int buttonPin = 5;        // Normally HIGH, Press = LOW
const int launchDetectPin = 8;
const int servoPin = 19;

int deploymentTimer = 3;        // Flight timer in seconds
int latchOpenAngle = 125;
int latchClosedAngle = 90;
int buttonPressCount = 0;
int sequenceState = 0;
int launchFlag = 0;
volatile bool led_state = LOW;

Servo myServo;  // create servo object to control a servo 
int servoPosition = 0;

int x = 0;

void setup() {
  // Setting up the pins for input/output
  pinMode(REDLED, OUTPUT);
  pinMode(GREENLED, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(launchDetectPin, INPUT_PULLUP);
  
  // Creating the servo and setting to the open latch position
  myServo.attach(servoPin);
  
  // Setting the status LEDs to initial, open latch position
  digitalWrite(REDLED, LOW);
  digitalWrite(GREENLED, LOW);
  
  attachInterrupt(digitalPinToInterrupt(buttonPin), onButtonPress, FALLING);
  attachInterrupt(digitalPinToInterrupt(launchDetectPin), onLaunch, FALLING);
}

void loop() {
    if (launchFlag == 0) {
        switch(sequenceState)
        {
        // 
            case 0:
                rocketAssemble();
                break;
            case 1:
                noseConeLatch();
                break;
            case 2:
                launchReady();
                break;
            case 3:
                launchAbort();
                break;
        }
    }
    if (launchFlag == 1)
    {
        launchDetected();
    }
}


void onButtonPress() {
    sequenceState++;
    sequenceState = sequenceState % 4;
}

void onLaunch() {
    if (sequenceState == 2)
    {
        launchFlag = 1;
    }
    // if (sequenceState == 2)
    // {
    //     launchFlag = 1;
    // }
    // launchFlag = 1;
    // myServo.write(180);
}

void launchDetected() {
    // volatile bool red_led_state = HIGH;
    deploymentTimer = deploymentTimer * 4;
    for (int i=0; i < deploymentTimer; i++)
    {
        digitalWrite(REDLED, led_state);
        digitalWrite(GREENLED, !led_state);
        delay(250);
        led_state = !led_state;
    }
    myServo.write(latchOpenAngle);
}

void rocketAssemble() {
    digitalWrite(GREENLED, LOW);
    digitalWrite(REDLED, LOW);
    myServo.write(latchOpenAngle);
}

void noseConeLatch() {
    myServo.write(latchClosedAngle);
    digitalWrite(GREENLED, HIGH);
    digitalWrite(REDLED, LOW);
}

void launchReady() {
    // if (digitalRead(launchDetectPin) = HIGH)  Only continue if launch detect is connected
    led_state = !led_state;
    digitalWrite(GREENLED, led_state);
    digitalWrite(REDLED, LOW);
    delay(500);
}
    
void launchAbort() {
    myServo.write(90);
    digitalWrite(GREENLED, HIGH);
    digitalWrite(REDLED, HIGH);
}