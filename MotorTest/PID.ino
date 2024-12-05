#include "driver/ledc.h"
#include "PID.h"


//setup_PID

//loop_PID

//internal

const int PWM_FREQ = 50;

// Smoothing using an exponential moving average
//smoothing factor of 0 is not smoothing and 1 is max smoothing(constant value)

const int resolution = 14; // Resolution for PWM duty cycle (14-bit) max: 2^14 - 1 = 16383
int maxDutyCycle = pow(2, resolution) - 1;

void configureMotorPins(Motor &motor){
  ledcAttach(motor.pwmPin, PWM_FREQ, resolution);
  pinMode(motor.encoderPin, INPUT_PULLUP);
  pinMode(motor.directionPin, OUTPUT);
  pinMode(motor.inverseDirectionPin, OUTPUT);
}

void setMotorDirection(Motor &motor, bool direction){
  motor.direction = direction;
  updateMotorDirection(motor);
}

void setMotorTargetFrequency(Motor &motor, float frequency){
  motor.targetFrequency = frequency;
}


void updateMotorDirection(Motor &motor) {
  digitalWrite(motor.directionPin, motor.direction ? LOW : HIGH);
  digitalWrite(motor.inverseDirectionPin, !motor.direction ? LOW : HIGH);
}

//valid speeds are 0 to 1
void updateMotorSpeed(Motor &motor, float speed) {
  int dutyCycle = speed * maxDutyCycle;
  ledcWrite(motor.pwmPin, dutyCycle);
}

void updateMotorSpeedToPIDOutput(Motor &motor){
  updateMotorSpeed(motor, motor.pidOutput);
}

void readMotorEncoder(Motor &motor){
  bool encoderState = digitalRead(motor.encoderPin);
  int currentMillis = millis();
  float timePeriod = (currentMillis - motor.lastEncoderMillis) / 1000.0;
  if (motor.lastEncoderState == HIGH && encoderState == LOW) {
    if(timePeriod > 0.005){
      motor.frequency = 1 / (timePeriod * ENCODER_STEPS_PER_REVOLUTION);
      motor.frequencySmooth = SMOOTHING_FACTOR * motor.frequencySmooth + (1 - SMOOTHING_FACTOR) * motor.frequency;
    }
    motor.lastEncoderMillis = currentMillis;
  }
  else if(timePeriod > 1){
    //if it hasn't moved in 1 second set the frequency to 0
    motor.frequency = 0;
    motor.frequencySmooth = SMOOTHING_FACTOR * motor.frequencySmooth + (1 - SMOOTHING_FACTOR) * motor.frequency;
    motor.lastEncoderMillis = currentMillis;
  }
  motor.lastEncoderState = encoderState;
}

// PID Constants
const float KP = 0.1;    // Decreased for slower response
const float KI = 0.03;   // Decreased to reduce oscillations
const float KD = 0.02;   // Decreased for gentler damping

// PID Variables

float calculateMotorPID(Motor &motor) {
    float error = motor.targetFrequency - motor.frequencySmooth;
    unsigned long currentTime = micros();
    float deltaTime = (currentTime - motor.lastPIDTime) / 1000000.0;// Convert to seconds
    
    // Only update if some time has passed
    if (deltaTime > 0) {
        motor.integral += error * deltaTime;
        float derivative = (error - motor.lastError) / deltaTime;

        float output = (KP * error) + (KI * motor.integral) + (KD * derivative);
        // Clamp output to valid range (0 to 1)
        output = constrain(output, 0.0, 1.0);
        
        motor.lastError = error;
        motor.lastPIDTime = currentTime;
        motor.pidOutput = output;
        return output;
    }
    Serial.printf("No time passed\n");
    motor.pidOutput = 0;
    return 0;
}

void setup_PID() {
  // Setup motor control pins and PWM
  configureMotorPins(motor1);
  configureMotorPins(motor2);
  configureMotorPins(motor3);
  configureMotorPins(motor4);
}

int lastPrintTime = millis();
bool timeToPrint(){
  return millis() - lastPrintTime > 1000;
}

void printMotorPID(Motor &motor, String motorName){
  Serial.printf("%s Frequency: %.2f\n", motorName.c_str(), motor.frequency);
  Serial.printf("%s Frequency Smooth: %.2f\n", motorName.c_str(), motor.frequencySmooth);
  Serial.printf("%s PID Output: %.2f\n", motorName.c_str(), motor.pidOutput);
}

void loop_PID_for_motor(Motor &motor){
  calculateMotorPID(motor);
  updateMotorSpeedToPIDOutput(motor);
  readMotorEncoder(motor);
}

void loop_PID() {
  // Add this at the start of loop() to print frequency

  loop_PID_for_motor(motor1);
  loop_PID_for_motor(motor2);
  loop_PID_for_motor(motor3);
  loop_PID_for_motor(motor4);

  if(timeToPrint()){
    printMotorPID(motor1, "Motor 1");
    printMotorPID(motor2, "Motor 2");
    printMotorPID(motor3, "Motor 3");
    printMotorPID(motor4, "Motor 4");

    lastPrintTime = millis();
  }
}