// Include the AccelStepper library:
#include <AccelStepper.h>
// Define stepper motor connections and motor interface type. Motor interface type must be set to 1 when using a driver:

// 1 -> means a stepper driver (with Step and Direction pins) (AccelStepper::DRIVER)
// 2 -> means a 2 wire stepper (2 pins required)
// 3 -> means a 3 wire stepper, such as HDD spindle (3 pins required)
// 4 -> means a 4 wire stepper (4 pins required)
// 6 -> means a 3 wire half stepper, such as HDD spindle (3 pins required)
// 8 -> means a 4 wire half stepper (4 pins required)
#define motorInterfaceType 1

// Arduino digital pin number for motor pin 1. Defaults to pin 2. For a AccelStepper::DRIVER (interface==1),
// this is the Step input to the driver. Low to high transition means to step)
#define stepPin 0

// Arduino digital pin number for motor pin 2. Defaults to pin 3. For a AccelStepper::DRIVER (interface==1),
// this is the Direction input the driver. High means forward.
#define dirPin 1

// How Many Step are we Rotating each Button trigger
#define StepsToRotate 3200

// The Speed we are using to rotate to each of the Steps
#define Speed 6000

// THe Maximum permissible speed
#define MaxSpeed 10000

// Create a new instance of the AccelStepper class:
AccelStepper stepper = AccelStepper(motorInterfaceType, stepPin, dirPin);

//--------------------------------
//  Button Parameters / Helpflags
//--------------------------------
struct Button {
  int Pin;
  int Reading;
  int ButtonState;
  int LastButtonState;
  unsigned long LastDebounceTime;
  unsigned long TimeDebounce;
};

//// Button which rotates the wheel forward
Button BtnStepForward = {2 , LOW, LOW, LOW , 0 , 50};
//// Button which rotates the wheel backward
Button BtnStepBackward = {4 , LOW, LOW, LOW , 0 , 50};


//-------------------------------------------
// Sertup Called from Main before going Loop
//-------------------------------------------
void setup() {
  // Set the maximum speed in steps per second:
  stepper.setMaxSpeed(MaxSpeed);

  // Assign Pins for Buttons
  pinMode(BtnStepForward.Pin,INPUT);
  pinMode(BtnStepBackward.Pin,INPUT);
}

//------------------------------
//  Button Step Forward Logic
//------------------------------
void BtnStepForwardProcess() {

  // Read the Pin where the Button is Wired
  BtnStepForward.Reading = digitalRead(BtnStepForward.Pin);

  // If the Button was pressed due to noise or pressing
  if (BtnStepForward.Reading != BtnStepForward.LastButtonState) {
    // Reset the Timer
    BtnStepForward.LastDebounceTime = millis();
  }

  // if this is true we have pressed the button at least for a ceartain amount of time
  if (millis() - BtnStepForward.LastDebounceTime > BtnStepForward.TimeDebounce) {

    // if the button state has changed:
    if (BtnStepForward.Reading != BtnStepForward.ButtonState) {
      BtnStepForward.ButtonState = BtnStepForward.Reading;

      // only move Motor Forward if Button state is high
      if (BtnStepForward.ButtonState == HIGH) {
        // Move Motor Forward
        // Set the current position to 0:
        stepper.setCurrentPosition(0);
        // Run the motor forward at 6000 steps/second until the motor reaches 3200 steps:
        while (stepper.currentPosition() != StepsToRotate)
        {
          stepper.setSpeed(Speed);
          stepper.runSpeed();
        }
      }
    }

  }
  // This will hold the last Button state
  BtnStepForward.LastButtonState =  BtnStepForward.Reading;
}

//------------------------------
//  Button Step Backward Logic
//------------------------------
void BtnStepBackwardProcess() {

  // Read the Pin where the Button is Wired
  BtnStepBackward.Reading = digitalRead(BtnStepBackward.Pin);

  // If the Button was pressed due to noise or pressing
  if (BtnStepBackward.Reading != BtnStepBackward.LastButtonState) {
    // Reset the Timer
    BtnStepBackward.LastDebounceTime = millis();
  }

  // If this is true we have pressed the button at least for a ceartain amount of time
  if (millis() - BtnStepBackward.LastDebounceTime > BtnStepBackward.TimeDebounce) {

    // if the button state has changed:
    if (BtnStepBackward.Reading != BtnStepBackward.ButtonState) {
      BtnStepBackward.ButtonState = BtnStepBackward.Reading;

      // only move Motor Backward if Button state is high
      if (BtnStepBackward.ButtonState == HIGH) {
        // Move Motor Backward
        // Reset the position to 0:
        stepper.setCurrentPosition(0);
        // Run the motor forward at -6000 steps/second until the motor reaches -3200 steps:
        while (stepper.currentPosition() != (-1 * StepsToRotate))
        {
          stepper.setSpeed(-1 * Speed);
          stepper.runSpeed();
        }
      }
    }

  }
  // This will hold the last Button state
  BtnStepBackward.LastButtonState =  BtnStepBackward.Reading;
}

//----------------------
// Loop Called from Main
//----------------------
void loop() {
        
  BtnStepForwardProcess();
  BtnStepBackwardProcess();

}
