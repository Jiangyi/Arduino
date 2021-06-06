// Include the Arduino Stepper Library
#include <Stepper.h>

enum StepperMotor {
  X = 0,
  Y,
  Z,
  NUM_STEPPERS,
};

// Number of steps per output rotation
const int stepsPerRevolution = 200;

const int stepsPerSWIncrement = stepsPerRevolution / 10;

const int speed = 60;

union Instruction
{
  uint8_t parts[4];
  int32_t value;
};

Instruction _readBytes;

// Create Instance of Stepper library
Stepper myStepper[NUM_STEPPERS] = {
  {stepsPerRevolution, 2, 3, 4, 5},
  {stepsPerRevolution, 6, 7, 8, 9},
  {stepsPerRevolution, 10, 11, 12, 13},
};

void setup()
{
	// set the speed at 60 rpm:
  for (int i = 0; i < NUM_STEPPERS; ++i) {
	  myStepper[i].setSpeed(speed);
  }

	// initialize the serial port:
	Serial.begin(115200);
}

void loop() 
{
  // send data only when you receive data:
  if (Serial.available() >= NUM_STEPPERS * sizeof(uint32_t)) {
    for (int i = 0; i < NUM_STEPPERS; ++i) {
        // read the incoming bytes:
        for (uint32_t j = 0; j < sizeof(uint32_t); ++j)
        {
          _readBytes.parts[j] = Serial.read();
          // if (Serial.readByte(reinterpret_cast<char*>(&_readBytes.parts[j]), sizeof(uint32_t)) != sizeof(uint32_t)) {
          //   Serial.println("Error: Number of bytes read does not equal sizeof(uint32_t)");
          //   return;
          // }
        }
        // Serial.println(_readBytes.value, DEC);
        // Serial.print("Stepping by: ");
        int numSteps = _readBytes.value * stepsPerSWIncrement;
        // Serial.println(numSteps, DEC);
        // Serial.print(" steps on motor ");
        // Serial.print(i, DEC);
        myStepper[i].step(numSteps);
    }
    Serial.write(42);
  }

  // for (int i = 0; i < NUM_STEPPERS; ++i) {
  //   // step one revolution in one direction:
  //   Serial.println("clockwise");
  //   myStepper[i].step(stepsPerRevolution);
  //   delay(500);

  //   // step one revolution in the other direction:
  //   Serial.println("counterclockwise");
  //   myStepper[i].step(-stepsPerRevolution);
  //   delay(500);
  // }

  // speed -= 5;
  // myStepper.setSpeed(speed);
}